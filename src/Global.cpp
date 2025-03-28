#include "Global.hpp"
#include "Supervisor.hpp"
#include "utils.hpp"
#include <limits.h>
#include <stdio.h>

namespace th08
{
DIFFABLE_STATIC(Rng, g_Rng);
DIFFABLE_STATIC(GameErrorContext, g_GameErrorContext);
DIFFABLE_STATIC(PbgArchive, g_PbgArchive);
DIFFABLE_STATIC(ZunMemory, g_ZunMemory);

Chain::~Chain()
{
}

ChainElem::ChainElem()
{
    m_Prev = NULL;
    m_Next = NULL;
    m_Callback = NULL;
    m_UnkPtr = this;
    m_AddedCallback = NULL;
    m_DeletedCallback = NULL;
    m_Priority = 0;
    m_IsHeapAllocated = 0;
}

ChainElem::~ChainElem()
{
    if (m_DeletedCallback != NULL)
        m_DeletedCallback(m_Arg);

    m_Prev = NULL;
    m_Next = NULL;
    m_Callback = NULL;
    m_AddedCallback = NULL;
    m_DeletedCallback = NULL;
}

Chain::Chain()
{
}

#pragma var_order(inCursor, outCursorBackup, i, out, outCursor, numUnencrypted, unused)
LPBYTE FileSystem::Decrypt(LPBYTE inData, i32 size, u8 xorValue, u8 xorValueInc, i32 chunkSize, i32 maxBytes)
{
    i32 i;
    LPBYTE outCursorBackup;

    i32 unused = 0;
    i32 numUnencrypted = (size % chunkSize < chunkSize / 4) ? size % chunkSize : 0;

    LPBYTE inCursor = inData;
    LPBYTE out = (LPBYTE)g_ZunMemory.Alloc(size, "d:\\cygwin\\home\\zun\\prog\\th08\\global.h");
    LPBYTE outCursor = out;

    if (out == NULL)
    {
        return inData;
    }

    numUnencrypted += size & 1;
    size -= numUnencrypted;

    while (size > 0 && maxBytes > 0)
    {
        if (size < chunkSize)
        {
            chunkSize = size;
        }

        outCursorBackup = outCursor;
        outCursor = &outCursor[chunkSize - 1];

        for (i = (chunkSize + 1) / 2; i > 0; i--, inCursor++)
        {
            *outCursor = *inCursor ^ xorValue;
            outCursor -= 2;
            xorValue += xorValueInc;
        }

        outCursor = &outCursorBackup[chunkSize - 2];

        for (i = chunkSize / 2; i > 0; i--, inCursor++)
        {
            *outCursor = *inCursor ^ xorValue;
            outCursor -= 2;
            xorValue += xorValueInc;
        }

        size -= chunkSize;
        outCursor = &outCursorBackup[chunkSize];
        maxBytes -= chunkSize;
    }

    size += numUnencrypted;
    if (size > 0)
    {
        memcpy(outCursor, inCursor, size);
    }

    return out;
}

struct DecryptParams
{
    u8 key;
    u8 xorValue;
    u8 xorValueInc;
    u8 unused;
    i32 chunkSize;
    i32 maxBytesToDecrypt;
};

DIFFABLE_STATIC_ARRAY_ASSIGN(DecryptParams, 8, g_DecryptParams) = {
    {0x5d, 0x1b, 0x37, 0xaa, 0x0040, 0x2800}, {0x74, 0x51, 0xe9, 0xbb, 0x0040, 0x3000},
    {0x71, 0xc1, 0x51, 0xcc, 0x1400, 0x2000}, {0x8a, 0x03, 0x19, 0xdd, 0x1400, 0x7800},
    {0x95, 0xab, 0xcd, 0xee, 0x0200, 0x1000}, {0xb7, 0x12, 0x34, 0xff, 0x0400, 0x2800},
    {0x9d, 0x35, 0x97, 0x11, 0x0080, 0x2800}, {0xaa, 0x99, 0x37, 0x77, 0x0400, 0x1000},
};

DIFFABLE_STATIC_ARRAY_ASSIGN(u8, 3, g_CryptSignature) = {0x85, 0xa4, 0xda};

#pragma var_order(rawData, decryptedData, i)
LPBYTE FileSystem::TryDecryptFromTable(LPBYTE inData, LPINT unused, i32 size)
{
    LPBYTE rawData = inData;
    LPBYTE decryptedData;

    if (rawData[0] == g_CryptSignature[0] - 0x20 && rawData[1] == g_CryptSignature[1] - 0x40 &&
        rawData[2] == g_CryptSignature[2] - 0x60)
    {
        u32 i = 0;
        while (rawData[3] != g_DecryptParams[i].key - (i << 4) - 0x10 && i < 8)
        {
            i++;
        }
        if (i >= 8)
        {
            return rawData;
        }

        // 4 bytes are skipped to exclude the encryption signature
        decryptedData = Decrypt(rawData + 4, size - 4, g_DecryptParams[i].xorValue, g_DecryptParams[i].xorValueInc,
                                g_DecryptParams[i].chunkSize, g_DecryptParams[i].maxBytesToDecrypt);
        g_ZunMemory.Free(inData);
        return decryptedData;
    }

    return rawData;
}

#pragma var_order(inCursor, i, out, outCursor, numUnencrypted, inCursorBackup, unused)
LPBYTE FileSystem::Encrypt(LPBYTE inData, i32 size, u8 xorValue, u8 xorValueInc, i32 chunkSize, i32 maxBytes)
{
    i32 i;
    LPBYTE inCursorBackup;

    i32 unused = 0;
    i32 numUnencrypted = (size % chunkSize < chunkSize / 4) ? size % chunkSize : 0;

    LPBYTE inCursor = inData;
    LPBYTE out = (LPBYTE)g_ZunMemory.Alloc(size, "d:\\cygwin\\home\\zun\\prog\\th08\\global.h");
    LPBYTE outCursor = out;

    if (out == NULL)
    {
        return inData;
    }

    numUnencrypted += size & 1;
    size -= numUnencrypted;

    while (size > 0 && maxBytes > 0)
    {
        if (size < chunkSize)
        {
            chunkSize = size;
        }

        inCursorBackup = inCursor;
        inCursor = &inCursor[chunkSize - 1];

        for (i = (chunkSize + 1) / 2; i > 0; i--, outCursor++)
        {
            *outCursor = *inCursor ^ xorValue;
            inCursor -= 2;
            xorValue += xorValueInc;
        }

        inCursor = &inCursorBackup[chunkSize - 2];

        for (i = chunkSize / 2; i > 0; i--, outCursor++)
        {
            *outCursor = *inCursor ^ xorValue;
            inCursor -= 2;
            xorValue += xorValueInc;
        }

        size -= chunkSize;
        inCursor = &inCursorBackup[chunkSize];
        maxBytes -= chunkSize;
    }

    size += numUnencrypted;
    if (size > 0)
    {
        memcpy(outCursor, inCursor, size);
    }

    return out;
}

#pragma var_order(unused, entryname, size, data, handle)
LPBYTE FileSystem::OpenFile(LPSTR path, i32 *fileSize, BOOL isExternalResource)
{
    char *entryname;
    DWORD size;
    LPBYTE data;
    HANDLE handle;
    i32 unused = -1;

    g_Supervisor.EnterCriticalSectionWrapper(2);

    if (!isExternalResource)
    {
        entryname = strrchr(path, '\\');
        if (entryname == NULL)
        {
            entryname = path;
        }
        else
        {
            entryname = entryname + 1;
        }
        entryname = strrchr(entryname, '/');
        if (entryname == (char *)0x0)
        {
            entryname = path;
        }
        else
        {
            entryname = entryname + 1;
        }
        size = g_PbgArchive.GetEntryDecompressedSize(entryname);
        if (fileSize != NULL)
        {
            *fileSize = size;
        }
        if (size == 0)
        {
            g_GameErrorContext.Fatal("error : %s is not found in arcfile.\n", entryname);
            goto error;
        }
        if (size != 0)
        {
            utils::DebugPrint("%s Decode ... \n", entryname);

            data = (LPBYTE)g_ZunMemory.Alloc(size, path);
            if (data == NULL)
            {
                goto error;
            }
            g_PbgArchive.ReadDecompressEntry(entryname, data);
            data = TryDecryptFromTable(data, fileSize, size);
            goto done;
        }
    }

    utils::DebugPrint("%s Load ... \n", path);

    handle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,
                         FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {
        utils::DebugPrint("error : %s is not found.\n", path);
        goto error;
    }

    size = GetFileSize(handle, NULL);
    data = (LPBYTE)g_ZunMemory.Alloc(size, path);
    if (data == NULL)
    {
        utils::DebugPrint("error : %s allocation error.\n", path);
        CloseHandle(handle);
        goto error;
    }

    ReadFile(handle, data, size, &size, NULL);
    if (fileSize != NULL)
    {
        *fileSize = size;
    }

    CloseHandle(handle);
    data = TryDecryptFromTable(data, fileSize, size);

done:
    g_Supervisor.LeaveCriticalSectionWrapper(2);
    return data;

error:
    g_Supervisor.LeaveCriticalSectionWrapper(2);
    return NULL;
}

BOOL FileSystem::CheckIfFileAlreadyExists(LPCSTR path)
{
    g_Supervisor.EnterCriticalSectionWrapper(2);

    HANDLE handle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,
                                FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
        g_Supervisor.LeaveCriticalSectionWrapper(2);
        return TRUE;
    }

    g_Supervisor.LeaveCriticalSectionWrapper(2);
    return FALSE;
}

#pragma var_order(numBytesWritten, handle, buffer)
int FileSystem::WriteDataToFile(LPCSTR path, LPVOID data, size_t size)
{
    LPSTR buffer;
    DWORD numBytesWritten;

    g_Supervisor.EnterCriticalSectionWrapper(2);

    HANDLE handle = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                       NULL, GetLastError(), LANG_USER_DEFAULT, (LPSTR)&buffer, 0, NULL);

        utils::DebugPrint("error : %s write error %s\n", path, buffer);
        LocalFree(buffer);
        g_Supervisor.LeaveCriticalSectionWrapper(2);
        return -1;
    }

    WriteFile(handle, data, size, &numBytesWritten, NULL);
    if (size != numBytesWritten)
    {
        CloseHandle(handle);
        utils::DebugPrint("error : %s write error\n", path);
        g_Supervisor.LeaveCriticalSectionWrapper(2);
        return -2;
    }

    CloseHandle(handle);
    utils::DebugPrint("%s write ...\n", path);
    g_Supervisor.LeaveCriticalSectionWrapper(2);
    return 0;
}

const char *GameErrorContext::Log(const char *fmt, ...)
{
    char tmpBuffer[0x2000];
    size_t tmpBufferSize;
    va_list args;

    va_start(args, fmt);
    g_Supervisor.EnterCriticalSectionWrapper(3);
    vsprintf(tmpBuffer, fmt, args);

    tmpBufferSize = strlen(tmpBuffer);

    if (m_BufferEnd + tmpBufferSize < &m_Buffer[sizeof(m_Buffer) - 1])
    {
        strcpy(m_BufferEnd, tmpBuffer);

        m_BufferEnd += tmpBufferSize;
        m_BufferEnd[0] = '\0';
    }

    va_end(args);

    g_Supervisor.LeaveCriticalSectionWrapper(3);
    return fmt;
}

const char *GameErrorContext::Fatal(const char *fmt, ...)
{
    char tmpBuffer[512];
    size_t tmpBufferSize;
    va_list args;

    va_start(args, fmt);
    g_Supervisor.EnterCriticalSectionWrapper(3);
    vsprintf(tmpBuffer, fmt, args);

    tmpBufferSize = strlen(tmpBuffer);

    if (m_BufferEnd + tmpBufferSize < &m_Buffer[sizeof(m_Buffer) - 1])
    {
        strcpy(m_BufferEnd, tmpBuffer);

        m_BufferEnd += tmpBufferSize;
        m_BufferEnd[0] = '\0';
    }

    va_end(args);

    m_ShowMessageBox = true;

    g_Supervisor.LeaveCriticalSectionWrapper(3);
    return fmt;
}

u16 Rng::GetRandomU16(void)
{
    u16 temp = (m_Seed ^ 0x9630) - 0x6553;
    m_Seed = (((temp & 0xc000) >> 14) + temp * 4) & 0xffff;
    m_GenerationCount++;
    return m_Seed;
}

u32 Rng::GetRandomU32(void)
{
    return GetRandomU16() << 16 | GetRandomU16();
}

f32 Rng::GetRandomF32(void)
{
    // XXX: Divisor is rounded is rounded to UINT_MAX+1 because of floating point
    // jank
    return (f32)GetRandomU32() / (f32)UINT_MAX;
}

f32 Rng::GetRandomF32Signed(void)
{
    // XXX: Divisor is rounded is rounded to INT_MAX+1 because of floating point
    // jank
    return (f32)GetRandomU32() / (f32)INT_MAX - 1.0f;
}

ZunMemory::ZunMemory()
{
    m_Unk4000 = 0;
}

ZunMemory::~ZunMemory()
{
    if (m_Unk4000)
    {
        for (i32 i = 0; i < ARRAY_SIZE_SIGNED(m_Unk0); i++)
        {
            if (m_Unk0[i] != NULL)
            {
                free(m_Unk0[i]);
            }
        }
    }
}

GameErrorContext::GameErrorContext()
{
    m_BufferEnd = m_Buffer;
    m_BufferEnd[0] = '\0';
    m_ShowMessageBox = false;
}

GameErrorContext::~GameErrorContext()
{
}
}; // namespace th08
