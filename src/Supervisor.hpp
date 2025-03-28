#pragma once

#include <d3d8.h>
#include <d3dx8math.h>
#include <dinput.h>

#include "Midi.hpp"
#include "diffbuild.hpp"
#include "inttypes.hpp"
#include "utils.hpp"

namespace th08
{
#define GAME_VERSION 0x80001
#define ZWAV_MAGIC 'VAWZ'

struct ControllerMapping
{
    i16 shotButton;
    i16 bombButton;
    i16 focusButton;
    i16 menuButton;
    i16 upButton;
    i16 downButton;
    i16 leftButton;
    i16 rightButton;
    i16 skipButton;
};

enum MusicMode
{
    OFF = 0,
    WAV = 1,
    MIDI = 2
};

enum Difficulty
{
    EASY,
    NORMAL,
    HARD,
    LUNATIC,
    EXTRA,
};

enum EffectQuality
{
    MINIMUM,
    MODERATE,
    MAXIMUM
};

struct GameConfigOpts
{
    u32 useD3dHwTextureBlending : 1;
    u32 dontUseVertexBuf : 1;
    u32 force16bitTextures : 1;
    u32 clearBackBufferOnRefresh : 1;
    u32 displayMinimumGraphics : 1;
    u32 suppressUseOfGoroudShading : 1;
    u32 disableDepthTest : 1;
    u32 force60Fps : 1;
    u32 disableColorCompositing : 1;
    u32 referenceRasterizerMode : 1;
    u32 disableFog : 1;
    u32 dontUseDirectInput : 1;
    u32 redrawHUDEveryFrame : 1;
    u32 preloadMusic : 1;
    u32 disableVsync : 1;
    u32 dontDetectTextDrawingBackground : 1;
};

struct GameConfiguration
{
    ControllerMapping controllerMapping;
    i32 version;
    i16 padXAxis;
    i16 padYAxis;
    u8 lifeCount;
    u8 bombCount;
    u8 colorMode16bit;
    u8 musicMode;
    u8 playSounds;
    u8 defaultDifficulty;
    u8 windowed;
    // 0 = fullspeed, 1 = 1/2 speed, 2 = 1/4 speed.
    u8 frameskipConfig;
    u8 effectQuality;
    u8 slowMode;
    u8 shotSlow;
    u8 musicVolume;
    u8 sfxVolume;
    i8 unk29[15];
    GameConfigOpts opts;
};

struct Supervisor
{
    ZunResult LoadConfig(char *configFile);
    void ThreadClose();

    u32 IsShotSlowEnabled()
    {
        return this->m_Cfg.shotSlow;
    }

    u32 ShouldForceBackbufferClear()
    {
        return this->m_Cfg.opts.displayMinimumGraphics | this->m_Cfg.opts.clearBackBufferOnRefresh;
    }

    u32 IsMusicPreloadEnabled()
    {
        return this->m_Cfg.opts.preloadMusic;
    }

    void EnterCriticalSectionWrapper(int id)
    {
        EnterCriticalSection(&m_CriticalSections[id]);
        m_LockCounts[id]++;
    }

    void LeaveCriticalSectionWrapper(int id)
    {
        LeaveCriticalSection(&m_CriticalSections[id]);
        m_LockCounts[id]--;
    }

    HINSTANCE m_hInstance;
    PDIRECT3D8 m_D3dIface;
    PDIRECT3DDEVICE8 m_D3dDevice;
    LPDIRECTINPUTDEVICE8A m_Keyboard;
    LPDIRECTINPUTDEVICE8A m_Controller;
    DIDEVCAPS m_ControllerCaps;

    u8 unk40[0x04];

    HWND m_HwndGameWindow;
    D3DXMATRIX m_ViewMatrix;
    D3DXMATRIX m_ProjectionMatrix;
    D3DVIEWPORT8 m_Viewport;
    D3DPRESENT_PARAMETERS m_PresentParameters;
    MidiTimer *m_MidiTimer;
    GameConfiguration m_Cfg;
    i32 m_CalcCount;
    i32 m_WantedState;
    i32 m_CurState;
    i32 m_WantedState2;
    unknown_fields(0x164, 0x10);
    u32 unk174;
    unknown_fields(0x178, 0x4);
    BOOL m_DisableVsync;
    unknown_fields(0x180, 0x8);
    f32 framerateMultiplier;
    MidiOutput *midiOutput;
    unknown_fields(0x190, 0x14);
    u32 m_Flags;
    DWORD m_TotalPlayTime;
    DWORD m_SystemTime;
    D3DCAPS8 m_D3dCaps;
    HANDLE m_Unk284;
    unknown_fields(0x288, 0x4);
    BOOL m_Unk28c;
    DWORD m_Unk290;
    unknown_fields(0x294, 0x4);
    CRITICAL_SECTION m_CriticalSections[4];
    u8 m_LockCounts[4];
    DWORD unk2fc;

    unknown_fields(0x300, 0x64);
};
C_ASSERT(sizeof(Supervisor) == 0x364);

DIFFABLE_EXTERN(Supervisor, g_Supervisor);
}; // namespace th08
