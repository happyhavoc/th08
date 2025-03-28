#pragma once

#include <d3d8.h>
#include <d3dx8math.h>
#include <dinput.h>

#include "Midi.hpp"
#include "diffbuild.hpp"
#include "inttypes.hpp"

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

    u32 ShouldForceBackbufferClear()
    {
        return this->cfg.opts.displayMinimumGraphics | this->cfg.opts.clearBackBufferOnRefresh;
    }

    u32 IsMusicPreloadEnabled()
    {
        return this->cfg.opts.preloadMusic;
    }

    void EnterCriticalSectionWrapper(int id)
    {
        EnterCriticalSection(&criticalSections[id]);
        criticalSectionLockCounts[id]++;
    }

    void LeaveCriticalSectionWrapper(int id)
    {
        LeaveCriticalSection(&criticalSections[id]);
        criticalSectionLockCounts[id]--;
    }

    HINSTANCE hInstance;
    PDIRECT3D8 d3dIface;
    PDIRECT3DDEVICE8 d3dDevice;
    LPDIRECTINPUTDEVICE8A keyboard;
    LPDIRECTINPUTDEVICE8A controller;
    DIDEVCAPS controllerCaps;

    u8 unk40[0x04];

    HWND hwndGameWindow;
    D3DXMATRIX viewMatrix;
    D3DXMATRIX projectionMatrix;
    D3DVIEWPORT8 viewport;
    D3DPRESENT_PARAMETERS presentParameters;
    MidiTimer *midiTimer;
    GameConfiguration cfg;
    i32 calcCount;
    i32 wantedState;
    i32 curState;
    i32 wantedState2;

    u8 unk164[0x18];

    i32 disableVsync;
    u8 unk180[0x8];

    f32 framerateMultiplier;
    MidiOutput *midiOutput;

    u8 unk190[0x14];

    u32 flags;
    DWORD totalPlayTime;
    DWORD systemTime;
    D3DCAPS8 d3dCaps;
    HANDLE unk284;

    u8 unk288[0x04];

    DWORD unk28c;
    DWORD unk290;

    u8 unk294[0x04];

    CRITICAL_SECTION criticalSections[4];
    u8 criticalSectionLockCounts[4];
    DWORD unk2fc;

    u8 unk300[0x64];
};
C_ASSERT(sizeof(Supervisor) == 0x364);

DIFFABLE_EXTERN(Supervisor, g_Supervisor);
}; // namespace th08
