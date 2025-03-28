#include "Supervisor.hpp"
#include "Global.hpp"
#include "i18n.hpp"
#include "utils.hpp"
#include <WinBase.h>

namespace th08
{
DIFFABLE_STATIC(Supervisor, g_Supervisor);

#pragma optimize("s", on)
#pragma var_order(fileSize, configFileBuffer, bgmHandle, bytesRead, bgmBuffer, bgmHandle2, bytesRead2, bgmBuffer2)
ZunResult Supervisor::LoadConfig(char *configFile)
{
    i32 bgmBuffer[4];
    i32 bgmBuffer2[4];

    HANDLE bgmHandle;
    HANDLE bgmHandle2;

    DWORD bytesRead;
    DWORD bytesRead2;

    u8 *configFileBuffer;
    i32 fileSize;

    memset(&g_Supervisor.cfg, 0, sizeof(GameConfiguration));
    configFileBuffer = FileSystem::OpenFile(configFile, &fileSize, true);
    if (configFileBuffer == NULL)
    {
        g_GameErrorContext.Log(TH_ERR_CONFIG_NOT_FOUND);
    SET_DEFAULT:
        g_Supervisor.cfg.lifeCount = 2;
        g_Supervisor.cfg.bombCount = 3;
        g_Supervisor.cfg.colorMode16bit = 0;
        g_Supervisor.cfg.version = GAME_VERSION;
        g_Supervisor.cfg.padXAxis = 600;
        g_Supervisor.cfg.padYAxis = 600;
        bgmHandle = CreateFileA("./thbgm.dat", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
        if (bgmHandle != INVALID_HANDLE_VALUE)
        {
            ReadFile(bgmHandle, bgmBuffer, 16, &bytesRead, NULL);
            CloseHandle(bgmHandle);
            if (bgmBuffer[0] != ZWAV_MAGIC || bgmBuffer[1] != 1 || bgmBuffer[2] != 0x800)
            {
                g_GameErrorContext.Fatal(TH_ERR_BGM_VERSION_MISMATCH);
                return ZUN_ERROR;
            }
            g_Supervisor.cfg.musicMode = WAV;
        }
        else
        {
            g_Supervisor.cfg.musicMode = MIDI;
            utils::GuiDebugPrint(TH_ERR_NO_WAVE_FILE);
        }
        g_Supervisor.cfg.playSounds = 1;
        g_Supervisor.cfg.defaultDifficulty = NORMAL;
        g_Supervisor.cfg.windowed = false;
        g_Supervisor.cfg.frameskipConfig = false;
        g_Supervisor.cfg.controllerMapping = g_ControllerMapping;
        g_Supervisor.cfg.effectQuality = MAXIMUM;
        g_Supervisor.cfg.slowMode = 0;
        g_Supervisor.cfg.shotSlow = 0;
        g_Supervisor.cfg.musicVolume = 100;
        g_Supervisor.cfg.sfxVolume = 80;
    }
    else
    {
        g_Supervisor.cfg = *(GameConfiguration *)configFileBuffer;
        free(configFileBuffer);
        bgmHandle2 = CreateFileA("./thbgm.dat", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
        if (bgmHandle2 != INVALID_HANDLE_VALUE)
        {
            ReadFile(bgmHandle2, bgmBuffer2, 16, &bytesRead2, NULL);
            CloseHandle(bgmHandle2);
            if (bgmBuffer2[0] != ZWAV_MAGIC || bgmBuffer2[1] != 1 || bgmBuffer2[2] != 0x800)
            {
                g_GameErrorContext.Fatal(TH_ERR_BGM_VERSION_MISMATCH);
                return ZUN_ERROR;
            }
        }
        if (g_Supervisor.cfg.lifeCount >= 7 || g_Supervisor.cfg.bombCount >= 4 ||
            g_Supervisor.cfg.colorMode16bit >= 2 || g_Supervisor.cfg.musicMode >= 3 ||
            g_Supervisor.cfg.defaultDifficulty >= 6 || g_Supervisor.cfg.playSounds >= 2 ||
            g_Supervisor.cfg.windowed >= 2 || g_Supervisor.cfg.frameskipConfig >= 3 ||
            g_Supervisor.cfg.effectQuality >= 3 || g_Supervisor.cfg.slowMode >= 2 || g_Supervisor.cfg.shotSlow >= 2 ||
            g_Supervisor.cfg.version != GAME_VERSION || fileSize != 60)
        {

            g_GameErrorContext.Log(TH_ERR_CONFIG_ABNORMAL);
            memset(&g_Supervisor.cfg, 0, sizeof(GameConfiguration));
            goto SET_DEFAULT;
        }
        g_ControllerMapping = g_Supervisor.cfg.controllerMapping;
    }

    g_Supervisor.cfg.opts.useD3dHwTextureBlending = true;
    if (this->cfg.opts.dontUseVertexBuf != false)
    {
        g_GameErrorContext.Log(TH_ERR_NO_VERTEX_BUFFER);
    }
    if (this->cfg.opts.disableFog != 0)
    {
        g_GameErrorContext.Log(TH_ERR_NO_FOG);
    }
    if (this->cfg.opts.force16bitTextures != false)
    {
        g_GameErrorContext.Log(TH_ERR_USE_16BIT_TEXTURES);
    }
    // This should be inlined
    if (this->ShouldForceBackbufferClear())
    {
        g_GameErrorContext.Log(TH_ERR_FORCE_BACKBUFFER_CLEAR);
    }
    if (this->cfg.opts.displayMinimumGraphics != false)
    {
        g_GameErrorContext.Log(TH_ERR_DONT_RENDER_ITEMS);
    }
    if (this->cfg.opts.suppressUseOfGoroudShading != false)
    {
        g_GameErrorContext.Log(TH_ERR_NO_GOURAUD_SHADING);
    }
    if (this->cfg.opts.disableDepthTest != false)
    {
        g_GameErrorContext.Log(TH_ERR_NO_DEPTH_TESTING);
    }
    this->disableVsync = false;
    this->cfg.opts.force60Fps = false;

    if (this->cfg.opts.disableColorCompositing != false)
    {
        g_GameErrorContext.Log(TH_ERR_NO_TEXTURE_COLOR_COMPOSITING);
    }
    if (this->cfg.windowed != false)
    {
        g_GameErrorContext.Log(TH_ERR_LAUNCH_WINDOWED);
    }
    if (this->cfg.opts.referenceRasterizerMode != false)
    {
        g_GameErrorContext.Log(TH_ERR_FORCE_REFERENCE_RASTERIZER);
    }
    if (this->cfg.opts.dontUseDirectInput != false)
    {
        g_GameErrorContext.Log(TH_ERR_DO_NOT_USE_DIRECTINPUT);
    }
    if (this->cfg.opts.redrawHUDEveryFrame != false)
    {
        g_GameErrorContext.Log(TH_ERR_REDRAW_HUD_EVERY_FRAME);
    }
    if (this->cfg.opts.preloadMusic != false)
    {
        g_GameErrorContext.Log(TH_ERR_PRELOAD_BGM);
    }
    if (this->cfg.opts.disableVsync != false)
    {
        g_GameErrorContext.Log(TH_ERR_NO_VSYNC);
        g_Supervisor.disableVsync = true;
    }
    if (this->cfg.opts.dontDetectTextDrawingBackground != false)
    {
        g_GameErrorContext.Log(TH_ERR_DONT_DETECT_TEXT_BG);
    }

    if (FileSystem::WriteDataToFile(configFile, &g_Supervisor.cfg, sizeof(GameConfiguration)) != 0)
    {
        g_GameErrorContext.Fatal(TH_ERR_FILE_CANNOT_BE_EXPORTED, configFile);
        g_GameErrorContext.Fatal(TH_ERR_FOLDER_HAS_WRITE_PROTECT_OR_DISK_FULL);
        return ZUN_ERROR;
    }

    return ZUN_SUCCESS;
}
#pragma optimize("", on)
}; // namespace th08
