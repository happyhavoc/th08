#define _WIN32_WINNT 0x0500

#include <d3dx8.h>
#include <shobjidl.h>
#include <shlguid.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "Background.hpp"
#include "diffbuild.hpp"
#include "i18n.hpp"
#include "inttypes.hpp"
#include "Global.hpp"
#include "Supervisor.hpp" // Official name: mother.hpp
#include "SprtCtrl.hpp"
#include "ZunBool.hpp"
#include "ZunColor.hpp"
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
    u8 m_UsesRelativePath; // Disables vsync when set
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
    static i32 CalcExecutableChecksum();
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

char *GameWindow::FormatCapability(char *capabilityName, u32 capabilityFlags, u32 mask, char *buf)
{
    // Who needs strcpy when you have sprintf?
    buf += sprintf(buf, capabilityName);

    if ((capabilityFlags & mask) == 0)
    {
        buf += sprintf(buf, TH_DBG_CAPABILITY_NOT_PRESENT);
    }
    else
    {
        buf += sprintf(buf, TH_DBG_CAPABILITY_PRESENT);
    }

    return buf;
}

#pragma var_order(fogVal, fogDensity)
void GameWindow::ResetRenderState()
{
    // Required to pass floats to SetTextureStageState without implicit conversion to int
    f32 fogDensity;
    f32 fogVal;

    if (!g_Supervisor.IsDepthTestDisabled())
    {
        g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_ZENABLE, true);
    }
    else
    {
        g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_ZENABLE, false);
    }

    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_LIGHTING, false);
    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_ALPHAREF, 4);
    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    
    if (!g_Supervisor.IsFogDisabled())
    {
        g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_FOGENABLE, true);
    }
    else
    {
        g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_FOGENABLE, false);
    }

    fogDensity = 1.0f;
    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_FOGDENSITY, *(u32 *) &fogDensity);
    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_FOGCOLOR, COLOR_LIGHT_GREY);

    fogVal = 1000.0f;
    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_FOGSTART, *(u32 *) &fogVal);
    fogVal = 5000.0f;
    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_FOGEND, *(u32 *) &fogVal);

    // Always evaluates true because Zun mistakenly used bitwise or instead of and
    // Doesn't matter, because it disables what it tried to test for anyway
    if (g_Supervisor.m_D3dCaps.RasterCaps | D3DPRASTERCAPS_ANTIALIASEDGES)
    {
        g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_EDGEANTIALIAS, false);
    }

    g_Supervisor.m_D3dDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, false);

    // Alpha texture settings
    if (!g_Supervisor.IsColorCompositingDisabled())
    {
        g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    }
    else
    {
        g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    }

    g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

    if (!g_Supervisor.IsVertexBufferDisabled())
    {
        g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
    }
    else
    {
        g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    }

    // Color texture settings
    if (!g_Supervisor.IsColorCompositingDisabled())
    {
        g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    }
    else
    {
        g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    }

    g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

    if (!g_Supervisor.IsVertexBufferDisabled())
    {
        g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    }
    else
    {
        g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    }

    g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTEXF_NONE);
    g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);
    g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
    g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSW, D3DTADDRESS_CLAMP);
    g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
    g_Supervisor.m_D3dDevice->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);

    if (g_SprtCtrl != NULL)
    {
        g_SprtCtrl->ClearBlendMode();
        g_SprtCtrl->ClearColorOp();
        g_SprtCtrl->ClearVertexShader();
        g_SprtCtrl->ClearTexture();
        g_SprtCtrl->ClearCameraSettings();
    }

    g_Background.m_SkyFogNeedsSetup = true;
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
        g_GameErrorContext.Fatal(TH_ERR_ALREADY_RUNNING);
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
                g_GameWindow.m_UsesRelativePath = true;
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

#pragma var_order(touhouWinThread, lockoutTime, foregroundWinThread)
void GameWindow::ActivateWindow(HWND hWnd)
{
    DWORD foregroundWinThread;
    u32 lockoutTime;
    DWORD touhouWinThread;

    foregroundWinThread = GetWindowThreadProcessId(GetForegroundWindow(), NULL);
    touhouWinThread = GetWindowThreadProcessId(hWnd, NULL);
    AttachThreadInput(touhouWinThread, foregroundWinThread, true);
    SystemParametersInfoA(SPI_GETFOREGROUNDLOCKTIMEOUT, 0, &lockoutTime, 0);
    SystemParametersInfoA(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (PVOID) 0, 0);
    SetActiveWindow(hWnd);
    SystemParametersInfoA(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, &lockoutTime, 0);
    AttachThreadInput(touhouWinThread, foregroundWinThread, false);
}

#pragma var_order(moduleFilenameBuf, i, fileSize, dataCursor, checksum, dataBase)
i32 GameWindow::CalcExecutableChecksum()
{
    u32 checksum;
    u32 *dataBase;
    u32 *dataCursor;
    i32 fileSize;
    i32 i;
    char moduleFilenameBuf[MAX_PATH + 1];

    if (GetModuleFileNameA(NULL, moduleFilenameBuf, ARRAY_SIZE(moduleFilenameBuf)))
    {
        checksum = 0;
        dataCursor = (u32 *) FileSystem::OpenFile(moduleFilenameBuf, &fileSize, true);
        dataBase = dataCursor;

        if (dataCursor == NULL)
        {
            return -1;
        }

        for (i = 0; i < (fileSize / 4) - 1; i++, dataCursor++)
        {
            checksum += *dataCursor;
        }

        utils::DebugPrint("main sum %d\r\n", checksum);
        g_ZunMemory.Free(dataBase);
        g_Supervisor.m_ExeChecksum = checksum;
        g_Supervisor.m_ExeSize = fileSize;

        return checksum;
    }

    return -1;
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
