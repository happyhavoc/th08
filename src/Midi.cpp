#include "inttypes.hpp"
#include <Windows.h>
#include <mmreg.h>
#include <mmsystem.h>

#include "Global.hpp"
#include "Midi.hpp"
#include "Supervisor.hpp"
#include "i18n.hpp"
#include "utils.hpp"

namespace th08
{
MidiDevice::MidiDevice()
{
    handle = NULL;
    m_DeviceId = 0;
}

MidiDevice::~MidiDevice()
{
    Close();
}

BOOL MidiDevice::OpenDevice(UINT uDeviceId)
{
    if (handle != NULL)
    {
        if (m_DeviceId != uDeviceId)
        {
            Close();
        }
        else
        {
            return FALSE;
        }
    }

    m_DeviceId = uDeviceId;

    return midiOutOpen(&handle, uDeviceId, (DWORD_PTR)g_Supervisor.m_HwndGameWindow, NULL, CALLBACK_WINDOW) !=
           MMSYSERR_NOERROR;
}

ZunResult MidiDevice::Close()
{
    if (handle == NULL)
    {
        return ZUN_ERROR;
    }

    midiOutReset(handle);
    midiOutClose(handle);
    handle = NULL;

    return ZUN_SUCCESS;
}

BOOL MidiDevice::SendLongMsg(LPMIDIHDR pmh)
{
    if (handle == NULL)
    {
        return FALSE;
    }
    else
    {
        if (midiOutPrepareHeader(handle, pmh, sizeof(*pmh)) != MMSYSERR_NOERROR)
        {
            return TRUE;
        }

        return midiOutLongMsg(handle, pmh, sizeof(*pmh)) != MMSYSERR_NOERROR;
    }
}

union MidiShortMsg {
    struct
    {
        u8 midiStatus;
        i8 firstByte;
        i8 secondByte;
        i8 unused;
    } msg;
    DWORD dwMsg;
};

BOOL MidiDevice::SendShortMsg(u8 midiStatus, u8 firstByte, u8 secondByte)
{
    MidiShortMsg pkt;

    if (handle == NULL)
    {
        return FALSE;
    }
    else
    {
        pkt.msg.midiStatus = midiStatus;
        pkt.msg.firstByte = firstByte;
        pkt.msg.secondByte = secondByte;
        return midiOutShortMsg(handle, pkt.dwMsg) != MMSYSERR_NOERROR;
    }
}

MidiTimer::MidiTimer()
{
    timeGetDevCaps(&m_TimeCaps, sizeof(TIMECAPS));
    m_TimerId = 0;
}

MidiTimer::~MidiTimer()
{
    StopTimerImpl();
    timeEndPeriod(m_TimeCaps.wPeriodMin);
}

UINT MidiTimer::StartTimerImpl(u32 delay, LPTIMECALLBACK cb, DWORD_PTR data)
{
    StopTimerImpl();
    timeBeginPeriod(m_TimeCaps.wPeriodMin);

    if (cb != NULL)
    {
        m_TimerId = timeSetEvent(delay, m_TimeCaps.wPeriodMin, cb, data, TIME_PERIODIC);
    }
    else
    {
        m_TimerId = timeSetEvent(delay, m_TimeCaps.wPeriodMin, (LPTIMECALLBACK)MidiTimer::DefaultTimerCallback,
                                 (DWORD_PTR)this, TIME_PERIODIC);
    }

    return m_TimerId;
}

BOOL MidiTimer::StopTimerImpl()
{
    if (m_TimerId != 0)
    {
        timeKillEvent(m_TimerId);
    }
    timeEndPeriod(m_TimeCaps.wPeriodMin);
    m_TimerId = 0;
    return TRUE;
}

void CALLBACK MidiTimer::DefaultTimerCallback(u32 uTimerID, u32 uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    MidiTimer *timer = (MidiTimer *)dwUser;
    timer->OnTimerElapsed();
}

u16 MidiOutput::Ntohs(u16 val)
{
    u8 tmp[2];

    tmp[0] = ((u8 *)&val)[1];
    tmp[1] = ((u8 *)&val)[0];

    return *(const u16 *)(&tmp);
}

u32 MidiOutput::SkipVariableLength(LPBYTE *curTrackDataCursor)
{
    u32 length;
    u8 tmp;

    length = 0;
    do
    {
        tmp = **curTrackDataCursor;
        *curTrackDataCursor = *curTrackDataCursor + 1;
        length = length * 0x80 + (tmp & 0x7f);
    } while ((tmp & 0x80) != 0);

    return length;
}

MidiOutput::MidiOutput()
{
    m_Tracks = NULL;
    m_Divisions = 0;
    m_Tempo = 0;
    m_NumTracks = 0;
    m_Unk2c4 = 0;
    m_FadeOutVolumeMultiplier = 0;
    m_FadeOutLastSetVolume = 0;
    m_Unk2d0 = 0;
    m_Unk2d4 = 0;
    m_Unk2d8 = 0;
    m_Unk2dc = 0;
    m_FadeOutFlag = FALSE;

    for (int i = 0; i < ARRAY_SIZE_SIGNED(m_MidiFileData); i++)
    {
        m_MidiFileData[i] = NULL;
    }

    for (int i = 0; i < ARRAY_SIZE_SIGNED(m_MidiHeaders); i++)
    {
        m_MidiHeaders[i] = NULL;
    }

    m_MidiFileIndex = -1;
    m_MidiHeadersCursor = 0;
}

MidiOutput::~MidiOutput()
{
    StopPlayback();
    ClearTracks();
    for (i32 i = 0; i < 32; i++)
    {
        ReleaseFileData(i);
    }
}

ZunResult MidiOutput::ReadFileData(int idx, LPCSTR path)
{
    if (m_MidiFileIndex == idx)
    {
        StopPlayback();
    }

    ReleaseFileData(idx);

    m_MidiFileData[idx] = FileSystem::OpenFile((LPSTR)path, NULL, false);
    if (m_MidiFileData[idx] == NULL)
    {
        g_GameErrorContext.Log(TH_ERR_MIDI_FAILED_TO_READ_FILE, path);
        return ZUN_ERROR;
    }

    return ZUN_SUCCESS;
}

void MidiOutput::ReleaseFileData(int idx)
{
    g_ZunMemory.Free(m_MidiFileData[idx]);
    m_MidiFileData[idx] = NULL;
}

void MidiOutput::ClearTracks()
{
    for (i32 trackIndex = 0; trackIndex < m_NumTracks; trackIndex++)
    {
        g_ZunMemory.Free(m_Tracks[trackIndex].trackData);
    }

    g_ZunMemory.Free(m_Tracks);
    m_Tracks = NULL;
    m_NumTracks = 0;
}

#pragma var_order(trackIdx, currentCursor, currentCursorTrack, fileData, hdrLength, hdrRaw, trackLength,               \
                  endOfHeaderPointer)
ZunResult MidiOutput::ParseFile(int fileIdx)
{
    u8 hdrRaw[8];
    u32 trackLength;
    LPBYTE currentCursor, currentCursorTrack, endOfHeaderPointer;
    i32 trackIdx;
    LPBYTE fileData;
    u32 hdrLength;

    ClearTracks();
    currentCursor = m_MidiFileData[fileIdx];
    fileData = currentCursor;
    if (currentCursor == NULL)
    {
        utils::DebugPrint(TH_ERR_MIDI_NOT_LOADED);
        return ZUN_ERROR;
    }

    // Read midi header chunk
    // First, read the header len
    memcpy(&hdrRaw, currentCursor, 8);

    // Get a pointer to the end of the header chunk
    currentCursor += sizeof(hdrRaw);
    hdrLength = MidiOutput::Ntohl(*(u32 *)(&hdrRaw[4]));

    endOfHeaderPointer = currentCursor;
    currentCursor += hdrLength;

    // Read the format. Only three values of format are specified:
    //  0: the file contains a single multi-channel track
    //  1: the file contains one or more simultaneous tracks (or MIDI outputs) of a
    //  sequence
    //  2: the file contains one or more sequentially independent single-track
    //  patterns
    m_Format = MidiOutput::Ntohs(*(u16 *)endOfHeaderPointer);

    // Read the divisions in this track. Note that this doesn't appear to support
    // "negative SMPTE format", which happens when the MSB is set.
    m_Divisions = MidiOutput::Ntohs(*(u16 *)(endOfHeaderPointer + 4));
    // Read the number of tracks in this midi file.
    m_NumTracks = MidiOutput::Ntohs(*(u16 *)(endOfHeaderPointer + 2));

    // Allocate m_Divisions * 32 bytes.
    m_Tracks = (MidiTrack *)g_ZunMemory.Alloc(sizeof(MidiTrack) * m_NumTracks, "midi");
    memset(m_Tracks, 0, sizeof(MidiTrack) * m_NumTracks);
    for (trackIdx = 0; trackIdx < m_NumTracks; trackIdx += 1)
    {
        currentCursorTrack = currentCursor;
        currentCursor += 8;

        // Read a track (MTrk) chunk.
        //
        // First, read the length of the chunk
        trackLength = MidiOutput::Ntohl(*(u32 *)&currentCursorTrack[4]);
        m_Tracks[trackIdx].trackLength = trackLength;
        m_Tracks[trackIdx].trackData = (LPBYTE)g_ZunMemory.Alloc(trackLength, "midi");
        m_Tracks[trackIdx].trackPlaying = TRUE;
        memcpy(m_Tracks[trackIdx].trackData, currentCursor, trackLength);
        currentCursor += trackLength;
    }
    m_Tempo = 1000000;
    m_MidiFileIndex = fileIdx;
    utils::DebugPrint(" midi open %d\n", fileIdx);
    return ZUN_SUCCESS;
}

ZunResult MidiOutput::LoadFile(LPCSTR midiPath)
{
    if (ReadFileData(0x1f, midiPath) != ZUN_SUCCESS)
    {
        return ZUN_ERROR;
    }

    ParseFile(0x1f);
    ReleaseFileData(0x1f);

    return ZUN_SUCCESS;
}

void MidiOutput::LoadTracks()
{
    i32 trackIndex;
    MidiTrack *track = m_Tracks;

    m_FadeOutVolumeMultiplier = 1.0;
    m_Unk2dc = 0;
    m_FadeOutFlag = FALSE;
    m_Volume = 0;
    m_Unk130 = 0;

    for (trackIndex = 0; trackIndex < m_NumTracks; trackIndex++, track++)
    {
        track->curTrackDataCursor = track->trackData;
        track->startTrackDataMaybe = track->curTrackDataCursor;
        track->trackPlaying = TRUE;
        track->trackLengthOther = MidiOutput::SkipVariableLength(&track->curTrackDataCursor);
    }
}

ZunResult MidiOutput::Play()
{
    if (m_Tracks == NULL)
    {
        return ZUN_ERROR;
    }

    LoadTracks();
    m_MidiOutDev.OpenDevice(MIDI_MAPPER);
    StartTimerImpl(1, NULL, NULL);
    utils::DebugPrint(" midi play\n");

    return ZUN_SUCCESS;
}

ZunResult MidiOutput::StopPlayback()
{
    if (m_Tracks == NULL)
    {
        return ZUN_ERROR;
    }

    for (i32 i = 0; i < ARRAY_SIZE_SIGNED(m_MidiHeaders); i++)
    {
        if (m_MidiHeaders[i] != NULL)
        {
            UnprepareHeader(m_MidiHeaders[i]);
        }
    }

    StopTimerImpl();
    m_MidiOutDev.Close();
    m_MidiFileIndex = -1;

    return ZUN_SUCCESS;
}

ZunResult MidiOutput::UnprepareHeader(LPMIDIHDR pmh)
{
    if (pmh == NULL)
    {
        utils::DebugPrint("error :\n");
    }

    if (m_MidiOutDev.handle == NULL)
    {
        utils::DebugPrint("error :\n");
    }

    for (i32 i = 0; i < ARRAY_SIZE_SIGNED(m_MidiHeaders); i++)
    {
        if (m_MidiHeaders[i] == pmh)
        {
            m_MidiHeaders[i] = NULL;
            goto success;
        }
    }

    return ZUN_ERROR;

success:
    MMRESULT res = midiOutUnprepareHeader(m_MidiOutDev.handle, pmh, sizeof(*pmh));
    if (res != MMSYSERR_NOERROR)
    {
        utils::DebugPrint("error :\n");
    }

    g_ZunMemory.Free(pmh->lpData);
    g_ZunMemory.Free(pmh);
    return ZUN_SUCCESS;
}

ZunResult MidiOutput::SetFadeOut(u32 ms)
{
    m_FadeOutVolumeMultiplier = 0.0;
    m_FadeOutInterval = ms;
    m_FadeOutElapsedMS = 0;
    m_Unk2dc = 0;
    m_FadeOutFlag = TRUE;

    return ZUN_SUCCESS;
}

#pragma var_order(trackIndex, local_14, trackLoaded)
void MidiOutput::OnTimerElapsed()
{
    BOOL trackLoaded = FALSE;
    ULONGLONG local_14 = m_Unk130 + (m_Volume * m_Divisions * 1000) / m_Tempo;
    if (m_FadeOutFlag != FALSE)
    {
        if (m_FadeOutElapsedMS < m_FadeOutInterval)
        {
            m_FadeOutVolumeMultiplier = 1.0f - (f32)m_FadeOutElapsedMS / (f32)m_FadeOutInterval;
            if ((u32)(m_FadeOutVolumeMultiplier * 128.0f) != m_FadeOutLastSetVolume)
            {
                FadeOutSetVolume(0);
            }
            m_FadeOutLastSetVolume = m_FadeOutVolumeMultiplier * 128.0f;
            m_FadeOutElapsedMS++;
        }
        else
        {
            m_FadeOutVolumeMultiplier = 0.0;
            return;
        }
    }
    i32 trackIndex;
    for (trackIndex = 0; trackIndex < m_NumTracks; trackIndex++)
    {
        if (m_Tracks[trackIndex].trackPlaying)
        {
            trackLoaded = TRUE;
            while (m_Tracks[trackIndex].trackPlaying)
            {
                if (m_Tracks[trackIndex].trackLengthOther <= local_14)
                {
                    ProcessMsg(&m_Tracks[trackIndex]);
                    local_14 = m_Unk130 + (m_Volume * m_Divisions * 1000 / m_Tempo);
                    continue;
                }
                break;
            }
        }
    }
    m_Volume++;
    if (!trackLoaded)
    {
        LoadTracks();
    }
}

#pragma var_order(nextTrackLength, idx, arg2, lVar5, opcodeLow, opcodeHigh, opcode, arg1, curTrackLength, midiHdr,     \
                  cVar1, unk24, local_2c, local_30)
void MidiOutput::ProcessMsg(MidiTrack *track)
{
    i32 lVar5;
    i32 curTrackLength, nextTrackLength;
    MidiTrack *local_30;
    MidiTrack *local_2c;
    u8 arg1, arg2;
    u8 opcode, opcodeHigh, opcodeLow;
    u8 cVar1;
    LPMIDIHDR midiHdr;
    i32 idx;
    i32 unk24;

    opcode = *track->curTrackDataCursor;
    if (opcode < MIDI_OPCODE_NOTE_OFF)
    {
        opcode = track->opcode;
    }
    else
    {
        track->curTrackDataCursor += 1;
    }
    // we AND the opcode to filter out the channel
    opcodeHigh = opcode & 0xf0;
    opcodeLow = opcode & 0x0f;

    switch (opcodeHigh)
    {
    case MIDI_OPCODE_SYSTEM_EXCLUSIVE:
        if (opcode == MIDI_OPCODE_SYSTEM_EXCLUSIVE)
        {
            if (m_MidiHeaders[m_MidiHeadersCursor] != NULL)
            {
                UnprepareHeader(m_MidiHeaders[m_MidiHeadersCursor]);
            }

            midiHdr = m_MidiHeaders[m_MidiHeadersCursor] = (LPMIDIHDR)g_ZunMemory.Alloc(sizeof(MIDIHDR), "midiHDR");
            curTrackLength = MidiOutput::SkipVariableLength(&track->curTrackDataCursor);
            memset(midiHdr, 0, sizeof(MIDIHDR));
            midiHdr->lpData = (LPSTR)g_ZunMemory.Alloc(curTrackLength + 1, "midiHDR->lpData");
            midiHdr->lpData[0] = MIDI_OPCODE_SYSTEM_EXCLUSIVE;
            midiHdr->dwFlags = 0;
            midiHdr->dwBufferLength = curTrackLength + 1;
            for (idx = 0; idx < curTrackLength; idx++)
            {
                midiHdr->lpData[idx + 1] = *track->curTrackDataCursor;
                track->curTrackDataCursor++;
            }
            if (m_MidiOutDev.SendLongMsg(midiHdr))
            {
                g_ZunMemory.Free(midiHdr->lpData);
                g_ZunMemory.Free(midiHdr);
                m_MidiHeaders[m_MidiHeadersCursor] = NULL;
            }
            m_MidiHeadersCursor += 1;
            m_MidiHeadersCursor = m_MidiHeadersCursor % 32;
        }
        else if (opcode == MIDI_OPCODE_SYSTEM_RESET)
        {
            // Meta-Event. In a MIDI file, SYSTEM_RESET gets reused as a
            // sort of escape code to introducde its own meta-events system,
            // which are events that make sense in the context of a MIDI
            // file, but not in the context of the MIDI protocol itself.
            cVar1 = *track->curTrackDataCursor;
            track->curTrackDataCursor += 1;
            curTrackLength = MidiOutput::SkipVariableLength(&track->curTrackDataCursor);
            // End of Track meta-event.
            if (cVar1 == 0x2f)
            {
                track->trackPlaying = 0;
                return;
            }
            // Set Tempo meta-event.
            if (cVar1 == 0x51)
            {
                m_Unk130 += (m_Volume * m_Divisions * 1000 / m_Tempo);
                m_Volume = 0;
                m_Tempo = 0;
                for (idx = 0; idx < curTrackLength; idx += 1)
                {
                    m_Tempo += m_Tempo * 0x100 + *track->curTrackDataCursor;
                    track->curTrackDataCursor += 1;
                }
                unk24 = 60000000 / m_Tempo;
                break;
            }
            track->curTrackDataCursor = track->curTrackDataCursor + curTrackLength;
        }
        break;
    case MIDI_OPCODE_NOTE_OFF:
    case MIDI_OPCODE_NOTE_ON:
    case MIDI_OPCODE_POLYPHONIC_AFTERTOUCH:
    case MIDI_OPCODE_MODE_CHANGE:
    case MIDI_OPCODE_PITCH_BEND_CHANGE:
        arg1 = *track->curTrackDataCursor;
        track->curTrackDataCursor += 1;
        arg2 = *track->curTrackDataCursor;
        track->curTrackDataCursor += 1;
        break;
    case MIDI_OPCODE_PROGRAM_CHANGE:
    case MIDI_OPCODE_CHANNEL_AFTERTOUCH:
        arg1 = *track->curTrackDataCursor;
        track->curTrackDataCursor += 1;
        arg2 = 0;
        break;
    }
    switch (opcodeHigh)
    {
    case MIDI_OPCODE_NOTE_ON:
        if (arg2 != 0)
        {
            arg1 += m_Unk2c4;
            m_Channels[opcodeLow].keyPressedFlags[arg1 >> 3] |= (u8)(1 << (arg1 & 7));
            break;
        }
    case MIDI_OPCODE_NOTE_OFF:
        arg1 += m_Unk2c4;
        m_Channels[opcodeLow].keyPressedFlags[arg1 >> 3] &= (u8)(~(1 << (arg1 & 7)));
        break;
    case MIDI_OPCODE_PROGRAM_CHANGE:
        // Program Change
        m_Channels[opcodeLow].instrument = arg1;
        break;
    case MIDI_OPCODE_MODE_CHANGE:
        switch (arg1)
        {
        case 0:
            // Bank Select
            m_Channels[opcodeLow].instrumentBank = arg2;
            break;
        case 7:
            // Channel Volume
            m_Channels[opcodeLow].channelVolume = arg2;
            lVar5 = (f32)arg2 * m_FadeOutVolumeMultiplier;
            if (lVar5 < 0)
            {
                lVar5 = 0;
            }
            else if (0x7f < lVar5)
            {
                lVar5 = 0x7f;
            }
            arg2 = m_Channels[opcodeLow].modifiedVolume = lVar5;
            break;
        case 91:
            // Effects 1 Depth
            m_Channels[opcodeLow].effectOneDepth = arg2;
            break;
        case 93:
            // Effects 3 Depth
            m_Channels[opcodeLow].effectThreeDepth = arg2;
            break;
        case 10:
            // Pan
            m_Channels[opcodeLow].pan = arg2;
            break;
        case 2:
            // Breath control
            for (local_2c = &m_Tracks[0], idx = 0; idx < m_NumTracks; idx += 1, local_2c += 1)
            {
                local_2c->startTrackDataMaybe = local_2c->curTrackDataCursor;
                local_2c->unk1c = local_2c->trackLengthOther;
            }
            m_TempoAtLoopPoint = m_Tempo;
            m_VolumeAtLoopPoint = m_Volume;
            m_Unk2f8 = m_Unk130;
            break;
        case 4:
            // Foot controller
            for (local_30 = &m_Tracks[0], idx = 0; idx < m_NumTracks; idx += 1, local_30 += 1)
            {
                local_30->curTrackDataCursor = (byte *)local_30->startTrackDataMaybe;
                local_30->trackLengthOther = local_30->unk1c;
            }
            m_Tempo = m_TempoAtLoopPoint;
            m_Volume = m_VolumeAtLoopPoint;
            m_Unk130 = m_Unk2f8;
        }
        break;
    }
    if (opcode < MIDI_OPCODE_SYSTEM_EXCLUSIVE)
    {
        m_MidiOutDev.SendShortMsg(opcode, arg1, arg2);
    }
    track->opcode = opcode;
    nextTrackLength = MidiOutput::SkipVariableLength(&track->curTrackDataCursor);
    track->trackLengthOther = track->trackLengthOther + nextTrackLength;
}

#pragma var_order(arg1, idx, volumeByte, midiStatus, volumeClamped)
void MidiOutput::FadeOutSetVolume(i32 volume)
{
    i32 volumeClamped;
    u32 volumeByte;
    i32 idx;
    i32 arg1;
    u32 midiStatus;

    if (m_Unk2d4 != 0)
    {
        return;
    }
    arg1 = 7;
    for (idx = 0; idx < ARRAY_SIZE_SIGNED(m_Channels); idx += 1)
    {
        midiStatus = (u8)(idx + 0xb0);
        volumeClamped = (i32)(m_Channels[idx].channelVolume * m_FadeOutVolumeMultiplier) + volume;
        if (volumeClamped < 0)
        {
            volumeClamped = 0;
        }
        else if (volumeClamped > 127)
        {
            volumeClamped = 127;
        }
        volumeByte = (u8)volumeClamped;
        m_MidiOutDev.SendShortMsg(midiStatus, arg1, volumeByte);
    }
    return;
}

void MidiTimer::StartTimer()
{
    StartTimerImpl(16, NULL, NULL);
}

void MidiTimer::StopTimer()
{
    StopTimerImpl();
}

}; // namespace th08
