#pragma once

#include <windows.h>
#include "diffbuild.hpp"
#include "inttypes.hpp"
#include "ZunBool.hpp"

namespace th08
{
// MSVC tries to align 64-bit types even on 32-bit builds, so the pack is required
#pragma pack(4)
struct GameWindow
{
    HWND m_Window;
    ZunBool m_WindowIsClosing; // Kept from previous games, but never set to true in IN
    ZunBool m_WindowIsActive;
    ZunBool m_WindowIsInactive;
    i8 m_FramesSinceRedraw;
    LARGE_INTEGER m_PCFrequency;
    u8 m_1C;
    ZunBool m_ScreenSaveActive;
    ZunBool m_LowPowerActive;
    ZunBool m_PowerOffActive;
    f64 m_CurTimestamp;
    f64 m_LastTimestamp;
    f64 m_LastFrameTime;
};
C_ASSERT(sizeof(GameWindow) == 0x44);

DIFFABLE_EXTERN(GameWindow, g_GameWindow)
}
