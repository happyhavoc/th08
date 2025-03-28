#pragma once

#include "Supervisor.hpp"
#include "ZunResult.hpp"
#include "diffbuild.hpp"
#include "inttypes.hpp"
#include "pbg/PbgArchive.hpp"
#include <windows.h>

namespace th08
{
enum ChainCallbackResult
{
    CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB = (unsigned int)0,
    CHAIN_CALLBACK_RESULT_CONTINUE = (unsigned int)1,
    CHAIN_CALLBACK_RESULT_EXECUTE_AGAIN = (unsigned int)2,
    CHAIN_CALLBACK_RESULT_BREAK = (unsigned int)3,
    CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS = (unsigned int)4,
    CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR = (unsigned int)5,
    CHAIN_CALLBACK_RESULT_RESTART_FROM_FIRST_JOB = (unsigned int)6,
};

typedef ChainCallbackResult (*ChainCallback)(void *);
typedef ZunResult (*ChainLifetimeCallback)(void *);

// TODO: rename to funcChainInf
class ChainElem
{
  public:
    ChainElem();
    ~ChainElem();

    void SetCallback(ChainCallback callback)
    {
        m_Callback = callback;
        m_AddedCallback = NULL;
        m_DeletedCallback = NULL;
    }

    short m_Priority;
    u16 m_IsHeapAllocated : 1;
    ChainCallback m_Callback;
    ChainLifetimeCallback m_AddedCallback;
    ChainLifetimeCallback m_DeletedCallback;
    struct ChainElem *m_Prev;
    struct ChainElem *m_Next;
    struct ChainElem *m_UnkPtr;
    void *m_Arg;
};

class Chain
{
  private:
    ChainElem m_CalcChain;
    ChainElem m_DrawChain;

    void ReleaseSingleChain(ChainElem *root);
    void CutImpl(ChainElem *to_remove);

  public:
    Chain();
    ~Chain();

    void Cut(ChainElem *to_remove);
    void Release();
    int AddToCalcChain(ChainElem *elem, int priority);
    int AddToDrawChain(ChainElem *elem, int priority);
    int RunDrawChain();
    int RunCalcChain();

    ChainElem *CreateElem(ChainCallback callback);
};

enum TouhouButton
{
    TH_BUTTON_SHOOT = 1 << 0,
    TH_BUTTON_BOMB = 1 << 1,
    TH_BUTTON_FOCUS = 1 << 2,
    TH_BUTTON_MENU = 1 << 3,
    TH_BUTTON_UP = 1 << 4,
    TH_BUTTON_DOWN = 1 << 5,
    TH_BUTTON_LEFT = 1 << 6,
    TH_BUTTON_RIGHT = 1 << 7,
    TH_BUTTON_SKIP = 1 << 8,
    TH_BUTTON_Q = 1 << 9,
    TH_BUTTON_S = 1 << 10,
    TH_BUTTON_HOME = 1 << 11,
    TH_BUTTON_ENTER = 1 << 12,
    TH_BUTTON_D = 1 << 13,
    TH_BUTTON_RESET = 1 << 14,

    TH_BUTTON_UP_LEFT = TH_BUTTON_UP | TH_BUTTON_LEFT,
    TH_BUTTON_UP_RIGHT = TH_BUTTON_UP | TH_BUTTON_RIGHT,
    TH_BUTTON_DOWN_LEFT = TH_BUTTON_DOWN | TH_BUTTON_LEFT,
    TH_BUTTON_DOWN_RIGHT = TH_BUTTON_DOWN | TH_BUTTON_RIGHT,
    TH_BUTTON_DIRECTION = TH_BUTTON_DOWN | TH_BUTTON_RIGHT | TH_BUTTON_UP | TH_BUTTON_LEFT,

    TH_BUTTON_SELECTMENU = TH_BUTTON_ENTER | TH_BUTTON_SHOOT,
    TH_BUTTON_RETURNMENU = TH_BUTTON_MENU | TH_BUTTON_BOMB,
    TH_BUTTON_WRONG_CHEATCODE =
        TH_BUTTON_SHOOT | TH_BUTTON_BOMB | TH_BUTTON_MENU | TH_BUTTON_Q | TH_BUTTON_S | TH_BUTTON_ENTER,
    TH_BUTTON_ANY = 0xFFFF,
};

namespace Controller
{
u16 GetJoystickCaps();
u32 SetButtonFromControllerInputs(u16 *outButtons, i16 controllerButtonToTest, u16 touhouButton,
                                  u32 inputButtons);

u32 SetButtonFromDirectInputJoystate(u16 *outButtons, i16 controllerButtonToTest, u16 touhouButton,
                                     u8 *inputButtons);

u16 GetControllerInput(u16 buttons);
u8 *GetControllerState();
u16 GetInput();
void ResetKeyboard();
}; // namespace Controller

namespace FileSystem
{
LPBYTE Decrypt(LPBYTE inData, i32 size, u8 xorValue, u8 xorValueInc, i32 chunkSize, i32 maxBytes);
LPBYTE TryDecryptFromTable(LPBYTE inData, LPINT unused, i32 size);
LPBYTE Encrypt(LPBYTE inData, i32 size, u8 xorValue, u8 xorValueInc, i32 chunkSize, i32 maxBytes);
LPBYTE OpenFile(LPSTR path, i32 *fileSize, BOOL isExternalResource);
BOOL CheckIfFileAlreadyExists(LPCSTR path);
int WriteDataToFile(LPCSTR path, LPVOID data, size_t size);
}; // namespace FileSystem

class GameErrorContext
{
  public:
    GameErrorContext();
    ~GameErrorContext();

    void ResetContext()
    {
        m_BufferEnd = m_Buffer;
        m_BufferEnd[0] = '\0';
    }

    void Flush()
    {
        if (m_BufferEnd != m_Buffer)
        {
            Log("---------------------------------------------------------- \n");

            if (m_ShowMessageBox)
            {
                MessageBoxA(NULL, m_Buffer, "", MB_ICONSTOP);
            }

            FileSystem::WriteDataToFile("./log.txt", m_Buffer, strlen(m_Buffer));
        }
    }

    const char *Log(const char *fmt, ...);
    const char *Fatal(const char *fmt, ...);

  private:
    char m_Buffer[0x2000];
    char *m_BufferEnd;
    i8 m_ShowMessageBox;
};

class Rng
{
  public:
    u16 GetRandomU16();
    u32 GetRandomU32();
    f32 GetRandomF32();
    f32 GetRandomF32Signed();

    void ResetGenerationCount()
    {
        m_GenerationCount = 0;
    }

    void SetSeed(u16 newSeed)
    {
        m_Seed = newSeed;
    }

    u16 GetSeed()
    {
        return m_Seed;
    }

    u16 GetRandomU16InRange(u16 range)
    {
        return range != 0 ? GetRandomU16() % range : 0;
    }

    u32 GetRandomU32InRange(u32 range)
    {
        return range != 0 ? GetRandomU32() % range : 0;
    }

    f32 GetRandomF32InRange(f32 range)
    {
        return GetRandomF32() * range;
    }

    f32 GetRandomF32SignedInRange(f32 range)
    {
        return GetRandomF32Signed() * range;
    }

  private:
    u16 m_Seed, m_SeedBackup;
    u32 m_GenerationCount;
};

class ZunMemory
{
  public:
    ZunMemory();
    ~ZunMemory();

    // NOTE: the default parameter for debugText is probably just __FILE__
    void *Alloc(size_t size, const char *debugText = "d:\\cygwin\\home\\zun\\prog\\th08\\global.h")
    {
        return malloc(size);
    }

    void Free(void *ptr)
    {
        free(ptr);
    }

    void *AddToRegistry(void *ptr, size_t size, char *name)
    {
#ifdef DEBUG
        m_bRegistryInUse = TRUE;
        for (i32 i = 0; i < ARRAY_SIZE_SIGNED(m_Registry); i++)
        {
            if (m_Registry[i] == NULL)
            {
                RegistryInfo *info = (RegistryInfo *)malloc(sizeof(*info));
                if (info != NULL)
                {
                    info->data = ptr;
                    info->size = size;
                    info->name = name;
                    m_Registry[i] = info;
                }
                break;
            }
        }
#endif
        return ptr;
    }

    void RemoveFromRegistry(VOID *ptr)
    {
#ifdef DEBUG
        for (i32 i = 0; i < ARRAY_SIZE_SIGNED(m_Registry); i++)
        {
            if (m_Registry[i] == ptr)
            {
                free(m_Registry[i]);
                m_Registry[i] = NULL;
                break;
            }
        }
#endif
    }

  private:
    struct RegistryInfo
    {
        void *data;
        size_t size;
        char *name;
    };

    RegistryInfo *m_Registry[0x1000];
    BOOL m_bRegistryInUse;
};

f32 AddNormalizeAngle(f32 a, f32 b);
void Rotate(D3DXVECTOR3 *outVector, D3DXVECTOR3 *point, f32 angle);

DIFFABLE_EXTERN(Rng, g_Rng);
DIFFABLE_EXTERN(GameErrorContext, g_GameErrorContext);
DIFFABLE_EXTERN(PbgArchive, g_PbgArchive);
DIFFABLE_EXTERN(ZunMemory, g_ZunMemory);
DIFFABLE_EXTERN(ControllerMapping, g_ControllerMapping);
}; // namespace th08
