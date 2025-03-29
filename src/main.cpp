#define _WIN32_WINNT 0x0500

#include <d3dx8.h>
#include <windows.h>
#include <string.h>
#include "diffbuild.hpp"
#include "ZunBool.hpp"
#include "ZunResult.hpp"

namespace th08
{
namespace main
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

    void Present();
};
C_ASSERT(sizeof(GameWindow) == 0x44);

f64 GetTimestamp();
ZunBool InitD3DInterface();
ZunBool CreateGameWindow(HINSTANCE hInstance);
LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
ZunBool InitD3DRendering();
void FormatD3DCapabilities(D3DCAPS8 *caps, char *buf);
char *FormatCapability(char *capabilityName, u32 capabilityFlags, u32 mask, char *buf);
void ResetRenderState();
ZunResult CheckForRunningGameInstance();
void ActivateWindow(HWND hWnd);
ZunResult CalcExecutableChecksum();
HRESULT ResolveIt(char *shortcutPath, char *dstPath, i32 maxPathLen);

DIFFABLE_STATIC(HANDLE, g_ExclusiveMutex);
DIFFABLE_STATIC(GameWindow, g_GameWindow);
}; // namespace main
}; // namespace th08

using namespace th08;
using namespace main;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR pCmdLine, int nCmdShow)
{
    return 0;
}
