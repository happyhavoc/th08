#define _WIN32_WINNT 0x0500

#include <d3dx8.h>
#include <shobjidl.h>
#include <shlguid.h>
#include <string.h>
#include <windows.h>
#include "diffbuild.hpp"
#include "Global.hpp"
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
    u8 m_LaunchedFromConsole; // Disables vsync when set
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
    static ZunResult CheckForRunningGameInstance(HINSTANCE hInstance);
    static void ActivateWindow(HWND hWnd);
    static ZunResult CalcExecutableChecksum();
    static ZunBool ResolveIt(char *shortcutPath, char *dstPath, i32 maxPathLen);
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

#pragma var_order(moduleFilenameBuf, startupInfo, consoleTitleBuf, fileExtension)
ZunResult GameWindow::CheckForRunningGameInstance(HINSTANCE hInstance)
{
    char consoleTitleBuf[MAX_PATH + 1];
    char *fileExtension;
    char moduleFilenameBuf[MAX_PATH + 1];
    STARTUPINFO startupInfo;

    g_ExclusiveMutex = CreateMutexA(NULL, TRUE, "Touhou 08 App");
    
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        g_GameErrorContext.Fatal("A");
        return ZUN_ERROR;
    }

    startupInfo.cb = sizeof(startupInfo);
    memset(&startupInfo.lpReserved, 0, sizeof(startupInfo) - 4); // Fill remaining struct members

    // GetModuleFileNameA will get the absolute path of the TH08 executable
    // GetConsoleTitleA will always fail(?) because TH08 isn't a console application. consoleTitleBuf is clobbered before being read anyway
    // GetStartupInfoA will return in lpTitle the absolute path of the executable/shortcut when launched from a graphical shell
    //   or the relative path the TH08 executable was launched with when launched from a console
    GetModuleFileNameA(NULL, moduleFilenameBuf, ARRAY_SIZE(moduleFilenameBuf));
    GetConsoleTitleA(consoleTitleBuf, ARRAY_SIZE(consoleTitleBuf));
    GetStartupInfoA(&startupInfo);

    if (startupInfo.lpTitle != NULL)
    {
        fileExtension = strrchr(startupInfo.lpTitle, '.');
        if (FileSystem::CheckIfFileAlreadyExists(startupInfo.lpTitle) && fileExtension != NULL)
        {
            if (_stricmp(fileExtension, ".lnk") == 0)
            {
                do
                {
                    ResolveIt(startupInfo.lpTitle, consoleTitleBuf, MAX_PATH);
                    fileExtension = strrchr(consoleTitleBuf, '.');
                } while ((_stricmp(fileExtension, ".lnk") == 0));
            }
            else
            {
                strcpy(consoleTitleBuf, startupInfo.lpTitle);
            }

            if (strcmp(moduleFilenameBuf, consoleTitleBuf) != 0)
            {
                // Won't be set true if TH08 was launched from console with absolute path
                g_GameWindow.m_LaunchedFromConsole = true;
            }
        }

        g_Supervisor.m_Flags &= 0xffffffbf;
    }
    else
    {
        g_Supervisor.m_Flags |= 0x00000040;
    }

    if (g_ExclusiveMutex == NULL)
    {
        return ZUN_ERROR;
    }
    else
    {
        return ZUN_SUCCESS;
    }
}

// Modified version of ResolveIt function used as an example in Microsoft's documentation
// https://web.archive.org/web/20250210164627/https://learn.microsoft.com/en-us/windows/win32/shell/links#resolving-a-shortcut
#pragma var_order(hres, retValue, psl, ppf, wPath, wfd)
ZunBool GameWindow::ResolveIt(char *shortcutPath, char *dstPath, i32 maxPathLen)
{
    HRESULT hres;
    IPersistFile *ppf;
    IShellLink *psl;
    ZunBool retValue;
    WIN32_FIND_DATA wfd;
    LPWSTR wPath;

    if (dstPath == NULL)
    {
        return false;
    }

    retValue = false;

    CoInitialize(NULL);
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *) &psl);
    if (SUCCEEDED(hres))
    {
        hres = psl->QueryInterface(IID_IPersistFile, (void **) &ppf);
        if (SUCCEEDED(hres))
        {
            wPath = new WCHAR[maxPathLen];
            // Presumably something that set hres was deleted here
            if (SUCCEEDED(hres))
            {
                MultiByteToWideChar(CP_ACP, 0, shortcutPath, -1, wPath, maxPathLen);
                hres = ppf->Load(wPath, STGM_READ);
                if (SUCCEEDED(hres))
                {
                    hres = psl->GetPath(dstPath, maxPathLen, &wfd, 0);
                    if (SUCCEEDED(hres))
                    {
                        retValue = true;
                    }
                }
            }

            delete wPath;
            ppf->Release();
        }

        psl->Release();
    }

    CoUninitialize();
    return retValue;
}
