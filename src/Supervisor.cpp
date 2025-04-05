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

    memset(&g_Supervisor.m_Cfg, 0, sizeof(GameConfiguration));
    configFileBuffer = FileSystem::OpenFile(configFile, &fileSize, true);
    if (configFileBuffer == NULL)
    {
        g_GameErrorContext.Log(TH_ERR_CONFIG_NOT_FOUND);
    SET_DEFAULT:
        g_Supervisor.m_Cfg.lifeCount = 2;
        g_Supervisor.m_Cfg.bombCount = 3;
        g_Supervisor.m_Cfg.colorMode16bit = 0;
        g_Supervisor.m_Cfg.version = GAME_VERSION;
        g_Supervisor.m_Cfg.padXAxis = 600;
        g_Supervisor.m_Cfg.padYAxis = 600;
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
            g_Supervisor.m_Cfg.musicMode = WAV;
        }
        else
        {
            g_Supervisor.m_Cfg.musicMode = MIDI;
            utils::GuiDebugPrint(TH_ERR_NO_WAVE_FILE);
        }
        g_Supervisor.m_Cfg.playSounds = 1;
        g_Supervisor.m_Cfg.defaultDifficulty = NORMAL;
        g_Supervisor.m_Cfg.windowed = false;
        g_Supervisor.m_Cfg.frameskipConfig = false;
        g_Supervisor.m_Cfg.controllerMapping = g_ControllerMapping;
        g_Supervisor.m_Cfg.effectQuality = MAXIMUM;
        g_Supervisor.m_Cfg.slowMode = 0;
        g_Supervisor.m_Cfg.shotSlow = 0;
        g_Supervisor.m_Cfg.musicVolume = 100;
        g_Supervisor.m_Cfg.sfxVolume = 80;
    }
    else
    {
        g_Supervisor.m_Cfg = *(GameConfiguration *)configFileBuffer;
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
        if (g_Supervisor.m_Cfg.lifeCount >= 7 || g_Supervisor.m_Cfg.bombCount >= 4 ||
            g_Supervisor.m_Cfg.colorMode16bit >= 2 || g_Supervisor.m_Cfg.musicMode >= 3 ||
            g_Supervisor.m_Cfg.defaultDifficulty >= 6 || g_Supervisor.m_Cfg.playSounds >= 2 ||
            g_Supervisor.m_Cfg.windowed >= 2 || g_Supervisor.m_Cfg.frameskipConfig >= 3 ||
            g_Supervisor.m_Cfg.effectQuality >= 3 || g_Supervisor.m_Cfg.slowMode >= 2 || g_Supervisor.m_Cfg.shotSlow >= 2 ||
            g_Supervisor.m_Cfg.version != GAME_VERSION || fileSize != 60)
        {

            g_GameErrorContext.Log(TH_ERR_CONFIG_ABNORMAL);
            memset(&g_Supervisor.m_Cfg, 0, sizeof(GameConfiguration));
            goto SET_DEFAULT;
        }
        g_ControllerMapping = g_Supervisor.m_Cfg.controllerMapping;
    }

    g_Supervisor.m_Cfg.opts.useSwTextureBlending = true; // Bit ignored from PCB onwards (HW blending always used)
    if (this->m_Cfg.opts.dontUseVertexBuf != false)
    {
        g_GameErrorContext.Log(TH_ERR_NO_VERTEX_BUFFER);
    }
    if (this->m_Cfg.opts.disableFog != 0)
    {
        g_GameErrorContext.Log(TH_ERR_NO_FOG);
    }
    if (this->m_Cfg.opts.force16bitTextures != false)
    {
        g_GameErrorContext.Log(TH_ERR_USE_16BIT_TEXTURES);
    }
    // This should be inlined
    if (this->ShouldForceBackbufferClear())
    {
        g_GameErrorContext.Log(TH_ERR_FORCE_BACKBUFFER_CLEAR);
    }
    if (this->m_Cfg.opts.displayMinimumGraphics != false)
    {
        g_GameErrorContext.Log(TH_ERR_DONT_RENDER_ITEMS);
    }
    if (this->m_Cfg.opts.suppressUseOfGoroudShading != false)
    {
        g_GameErrorContext.Log(TH_ERR_NO_GOURAUD_SHADING);
    }
    if (this->m_Cfg.opts.disableDepthTest != false)
    {
        g_GameErrorContext.Log(TH_ERR_NO_DEPTH_TESTING);
    }
    this->m_DisableVsync = false;
    this->m_Cfg.opts.force60Fps = false;

    if (this->m_Cfg.opts.disableColorCompositing != false)
    {
        g_GameErrorContext.Log(TH_ERR_NO_TEXTURE_COLOR_COMPOSITING);
    }
    if (this->m_Cfg.windowed != false)
    {
        g_GameErrorContext.Log(TH_ERR_LAUNCH_WINDOWED);
    }
    if (this->m_Cfg.opts.referenceRasterizerMode != false)
    {
        g_GameErrorContext.Log(TH_ERR_FORCE_REFERENCE_RASTERIZER);
    }
    if (this->m_Cfg.opts.dontUseDirectInput != false)
    {
        g_GameErrorContext.Log(TH_ERR_DO_NOT_USE_DIRECTINPUT);
    }
    if (this->m_Cfg.opts.redrawHUDEveryFrame != false)
    {
        g_GameErrorContext.Log(TH_ERR_REDRAW_HUD_EVERY_FRAME);
    }
    if (this->m_Cfg.opts.preloadMusic != false)
    {
        g_GameErrorContext.Log(TH_ERR_PRELOAD_BGM);
    }
    if (this->m_Cfg.opts.disableVsync != false)
    {
        g_GameErrorContext.Log(TH_ERR_NO_VSYNC);
        g_Supervisor.m_DisableVsync = true;
    }
    if (this->m_Cfg.opts.dontDetectTextDrawingBackground != false)
    {
        g_GameErrorContext.Log(TH_ERR_DONT_DETECT_TEXT_BG);
    }

    if (FileSystem::WriteDataToFile(configFile, &g_Supervisor.m_Cfg, sizeof(GameConfiguration)) != 0)
    {
        g_GameErrorContext.Fatal(TH_ERR_FILE_CANNOT_BE_EXPORTED, configFile);
        g_GameErrorContext.Fatal(TH_ERR_FOLDER_HAS_WRITE_PROTECT_OR_DISK_FULL);
        return ZUN_ERROR;
    }

    return ZUN_SUCCESS;
}

void Supervisor::ThreadClose()
{
    if (m_Unk284 != NULL)
    {
        utils::GuiDebugPrint("info : Sub Thread Close Request\n");
        m_Unk28c = TRUE;

        while (WaitForSingleObject(m_Unk284, 1000) == WAIT_TIMEOUT)
            Sleep(1);

        CloseHandle(m_Unk284);
        m_Unk284 = NULL;
        m_Unk28c = FALSE;
    }
}

void Supervisor::InitializeCriticalSections() {
    for (u32 i = 0; i < ARRAY_SIZE_SIGNED(m_CriticalSections); i++)
    {
        InitializeCriticalSection(&m_CriticalSections[i]);
    }
}
#pragma optimize("", on)
}; // namespace th08
