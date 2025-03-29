#define _WIN32_WINNT 0x0500

#include <d3dx8.h>
#include <windows.h>
#include <string.h>
#include "diffbuild.hpp"
#include "ZunBool.hpp"
#include "ZunResult.hpp"

namespace th08
{
enum RenderResult {
    RENDER_RESULT_KEEP_RUNNING = 0,
    RENDER_RESULT_EXIT_SUCCESS = 1,
    RENDER_RESULT_EXIST_ERROR = 2
};

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
    u8 m_1C; // Set according window and console names? Disables vsync if set
    ZunBool m_ScreenSaveActive;
    ZunBool m_LowPowerActive;
    ZunBool m_PowerOffActive;
    f64 m_CurTimestamp;
    f64 m_LastTimestamp;
    f64 m_LastFrameTime;

    GameWindow() {
        memset(this, 0, sizeof(*this));
    }

    RenderResult Render();
    static void Present();
    f64 GetTimestamp();
    static ZunBool InitD3DInterface();
    static ZunBool CreateGameWindow(HINSTANCE hInstance);
    static LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static ZunBool InitD3DRendering();
    static void FormatD3DCapabilities(D3DCAPS8 *caps, char *buf);
    static char *FormatCapability(char *capabilityName, u32 capabilityFlags, u32 mask, char *buf);
    static void ResetRenderState();
    static ZunResult CheckForRunningGameInstance();
    static void ActivateWindow(HWND hWnd);
    static ZunResult CalcExecutableChecksum();
    static HRESULT ResolveIt(char *shortcutPath, char *dstPath, i32 maxPathLen);
};
C_ASSERT(sizeof(GameWindow) == 0x44);

DIFFABLE_STATIC(HANDLE, g_ExclusiveMutex);
DIFFABLE_STATIC(GameWindow, g_GameWindow);
}; // namespace th08

using namespace th08;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR pCmdLine, int nCmdShow)
{
    return 0;
}
