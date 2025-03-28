#include "Global.hpp"
#include "Supervisor.hpp"
#include "utils.hpp"
#include "ZunMath.hpp"
#include <limits.h>
#include <stdio.h>
#include "i18n.hpp"

namespace th08
{
DIFFABLE_STATIC(Rng, g_Rng)
DIFFABLE_STATIC(GameErrorContext, g_GameErrorContext)
DIFFABLE_STATIC(PbgArchive, g_PbgArchive)
DIFFABLE_STATIC(ZunMemory, g_ZunMemory)
DIFFABLE_STATIC(JOYCAPSA, g_JoystickCaps)
DIFFABLE_STATIC(u16, g_FocusButtonConflictState)

Chain::~Chain()
{
}

ChainElem::ChainElem()
{
    m_Prev = NULL;
    m_Next = NULL;
    m_Callback = NULL;
    m_UnkPtr = this;
    m_AddedCallback = NULL;
    m_DeletedCallback = NULL;
    m_Priority = 0;
    m_IsHeapAllocated = false;
}

ChainElem::~ChainElem()
{
    if (m_DeletedCallback != NULL)
        m_DeletedCallback(m_Arg);

    m_Prev = NULL;
    m_Next = NULL;
    m_Callback = NULL;
    m_AddedCallback = NULL;
    m_DeletedCallback = NULL;
}

Chain::Chain()
{
}

#pragma var_order(cur, res)
int Chain::AddToCalcChain(ChainElem *elem, int priority)
{
    ChainElem *cur = &m_CalcChain;
    int res = 0;

    if (elem->m_AddedCallback != NULL)
    {
        res = elem->m_AddedCallback(elem->m_Arg);
        elem->m_AddedCallback = NULL;
    }

    g_Supervisor.EnterCriticalSectionWrapper(0);
    elem->m_Priority = priority;

    while (cur->m_Next != NULL)
    {
        if (cur->m_Priority > priority)
            break;
        cur = cur->m_Next;
    }

    if (cur->m_Priority > priority)
    {
        elem->m_Next = cur;
        elem->m_Prev = cur->m_Prev;

        if (elem->m_Prev != NULL)
            elem->m_Prev->m_Next = elem;

        cur->m_Prev = elem;
    }
    else
    {
        elem->m_Next = NULL;
        elem->m_Prev = cur;
        cur->m_Next = elem;
    }

    g_Supervisor.LeaveCriticalSectionWrapper(0);

    return res;
}

#pragma var_order(cur, res)
int Chain::AddToDrawChain(ChainElem *elem, int priority)
{
    ChainElem *cur = &m_DrawChain;
    int res = 0;

    if (elem->m_AddedCallback != NULL)
    {
        res = elem->m_AddedCallback(elem->m_Arg);
        elem->m_AddedCallback = NULL;
    }

    g_Supervisor.EnterCriticalSectionWrapper(0);
    elem->m_Priority = priority;

    while (cur->m_Next != NULL)
    {
        if (cur->m_Priority > priority)
            break;
        cur = cur->m_Next;
    }

    if (cur->m_Priority > priority)
    {
        elem->m_Next = cur;
        elem->m_Prev = cur->m_Prev;

        if (elem->m_Prev != NULL)
            elem->m_Prev->m_Next = elem;

        cur->m_Prev = elem;
    }
    else
    {
        elem->m_Next = NULL;
        elem->m_Prev = cur;
        cur->m_Next = elem;
    }

    g_Supervisor.LeaveCriticalSectionWrapper(0);

    return res;
}

#pragma var_order(current, updatedCount, result, tmp1)
int Chain::RunCalcChain()
{
    ChainElem *tmp1;
    ChainElem *current;
    int updatedCount;
    ChainCallbackResult result;

    g_Supervisor.EnterCriticalSectionWrapper(0);

restart_from_first_job:
    updatedCount = 0;
    current = &m_CalcChain;

    while (current != NULL)
    {
        if (current->m_Callback != NULL)
        {
        execute_again:
            g_Supervisor.LeaveCriticalSectionWrapper(0);
            result = current->m_Callback(current->m_Arg);
            g_Supervisor.EnterCriticalSectionWrapper(0);

            switch (result)
            {
            case CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB:
                tmp1 = current;
                current = current->m_Next;
                Cut(tmp1);

                updatedCount++;
                continue;

            case CHAIN_CALLBACK_RESULT_EXECUTE_AGAIN:
                goto execute_again;

            case CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS:
                updatedCount = 0;
                goto loop_exit;

            case CHAIN_CALLBACK_RESULT_BREAK:
                updatedCount = 1;
                goto loop_exit;

            case CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR:
                updatedCount = -1;
                goto loop_exit;

            case CHAIN_CALLBACK_RESULT_RESTART_FROM_FIRST_JOB:
                goto restart_from_first_job;

            default:
                break;
            }

            updatedCount++;
        }

        current = current->m_Next;
    }

loop_exit:
    g_Supervisor.LeaveCriticalSectionWrapper(0);
    return updatedCount;
}

void Chain::ReleaseSingleChain(ChainElem *root)
{
    // NOTE: Those names are like this to get perfect stack frame matching
    // TODO: Give meaningfull names that still match.
    ChainElem a0;
    ChainElem *current;
    ChainElem *tmp;
    ChainElem *wasNext;

    tmp = (ChainElem *)g_ZunMemory.AddToRegistry(new ChainElem(), sizeof(ChainElem), "funcChainInf");
    a0.m_Next = tmp;

    current = root;
    while (current != NULL)
    {
        tmp->m_UnkPtr = current;
        tmp->m_Next = (ChainElem *)g_ZunMemory.AddToRegistry(new ChainElem(), sizeof(ChainElem), "funcChainInf");
        tmp = tmp->m_Next;
        current = current->m_Next;
    }

    current = &a0;
    while (current != NULL)
    {
        Cut(current->m_UnkPtr);
        current = current->m_Next;
    }

    tmp = a0.m_Next;

    while (tmp != NULL)
    {
        wasNext = tmp->m_Next;
        g_ZunMemory.RemoveFromRegistry(tmp);
        delete tmp;
        tmp = NULL;
        tmp = wasNext;
    }
}

#pragma var_order(current, updatedCount, result, tmp1)
int Chain::RunDrawChain()
{
    ChainElem *tmp1;
    ChainElem *current;
    int updatedCount;
    ChainCallbackResult result;

    updatedCount = 0;
    current = &m_DrawChain;

    g_Supervisor.EnterCriticalSectionWrapper(0);

    while (current != NULL)
    {
        if (current->m_Callback != NULL)
        {
        execute_again:
            g_Supervisor.LeaveCriticalSectionWrapper(0);
            result = current->m_Callback(current->m_Arg);
            g_Supervisor.EnterCriticalSectionWrapper(0);

            switch (result)
            {
            case CHAIN_CALLBACK_RESULT_CONTINUE_AND_REMOVE_JOB:
                tmp1 = current;
                current = current->m_Next;
                Cut(tmp1);

                updatedCount++;
                continue;

            case CHAIN_CALLBACK_RESULT_EXECUTE_AGAIN:
                goto execute_again;

            case CHAIN_CALLBACK_RESULT_EXIT_GAME_SUCCESS:
                updatedCount = 0;
                goto loop_exit;

            case CHAIN_CALLBACK_RESULT_BREAK:
                updatedCount = 1;
                goto loop_exit;

            case CHAIN_CALLBACK_RESULT_EXIT_GAME_ERROR:
                updatedCount = -1;
                goto loop_exit;

            default:
                break;
            }

            updatedCount++;
        }

        current = current->m_Next;
    }

loop_exit:
    g_Supervisor.LeaveCriticalSectionWrapper(0);
    return updatedCount;
}

void Chain::Release()
{
    g_Supervisor.ThreadClose();
    ReleaseSingleChain(&m_CalcChain);
    ReleaseSingleChain(&m_DrawChain);
}

ChainElem *Chain::CreateElem(ChainCallback callback)
{
    ChainElem *elem = (ChainElem *)g_ZunMemory.AddToRegistry(new ChainElem(), sizeof(ChainElem), "funcChainInf");

    elem->SetCallback(callback);
    elem->m_IsHeapAllocated = true;

    return elem;
}

void Chain::Cut(ChainElem *to_remove)
{
    g_Supervisor.EnterCriticalSectionWrapper(0);
    CutImpl(to_remove);
    g_Supervisor.LeaveCriticalSectionWrapper(0);
}

void Chain::CutImpl(ChainElem *to_remove)
{
    BOOL isDrawChain;
    ChainElem *tmp;

    isDrawChain = FALSE;

    if (to_remove == NULL)
        return;

    tmp = &m_CalcChain;

    while (tmp != NULL)
    {
        if (tmp == to_remove)
            goto destroy_elem;

        tmp = tmp->m_Next;
    }

    isDrawChain = TRUE;

    tmp = &m_DrawChain;
    while (tmp != NULL)
    {
        if (tmp == to_remove)
            goto destroy_elem;

        tmp = tmp->m_Next;
    }

    return;

destroy_elem:
    if (to_remove->m_Prev != NULL)
    {
        to_remove->m_Callback = NULL;
        to_remove->m_Prev->m_Next = to_remove->m_Next;

        if (to_remove->m_Next != NULL)
        {
            to_remove->m_Next->m_Prev = to_remove->m_Prev;
        }

        to_remove->m_Prev = NULL;
        to_remove->m_Next = NULL;

        if (to_remove->m_IsHeapAllocated)
        {
            g_Supervisor.LeaveCriticalSectionWrapper(0);
            g_ZunMemory.RemoveFromRegistry(to_remove);
            delete to_remove;
            to_remove = NULL;
            g_Supervisor.EnterCriticalSectionWrapper(0);
        }
        else
        {
            if (to_remove->m_DeletedCallback != NULL)
            {
                ChainLifetimeCallback callback = to_remove->m_DeletedCallback;
                to_remove->m_DeletedCallback = NULL;
                g_Supervisor.LeaveCriticalSectionWrapper(0);
                callback(to_remove->m_Arg);
                g_Supervisor.EnterCriticalSectionWrapper(0);
            }
        }
    }
}

DIFFABLE_STATIC_ASSIGN(ControllerMapping, g_ControllerMapping) = {0, 1, 2, 4, -1, -1, -1, -1, 3};

u16 Controller::GetJoystickCaps(void)
{
    JOYINFOEX pji;

    pji.dwSize = sizeof(JOYINFOEX);
    pji.dwFlags = JOY_RETURNALL;

    if (joyGetPosEx(0, &pji) != MMSYSERR_NOERROR)
    {
        g_GameErrorContext.Log(TH_ERR_NO_PAD_FOUND);
        return 1;
    }

    joyGetDevCapsA(0, &g_JoystickCaps, sizeof(g_JoystickCaps));
    return 0;
}

#define JOYSTICK_MIDPOINT(min, max) ((min + max) / 2)
#define JOYSTICK_BUTTON_PRESSED(button, x, y) (x > y ? button : 0)
#define JOYSTICK_BUTTON_PRESSED_INVERT(button, x, y) (x < y ? button : 0)
#define KEYBOARD_KEY_PRESSED(button, x) keyboardState[x] & 0x80 ? button : 0
#define KEYBOARD_KEY_PRESSED2(button, x) keyboardState[x] & 0x800 ? button : 0

u16 Controller::GetControllerInput(u16 buttons)
{
    // NOTE: Those names are like this to get perfect stack frame matching
    // TODO: Give meaningfull names that still match.
    JOYINFOEX aa;
    u32 ab;
    u32 ac;
    DIJOYSTATE2 a0;
    u32 a2;
    HRESULT aaa;

    if (g_Supervisor.m_Controller == NULL)
    {
        memset(&aa, 0, sizeof(aa));
        aa.dwSize = sizeof(JOYINFOEX);
        aa.dwFlags = JOY_RETURNALL;

        if (joyGetPosEx(0, &aa) != MMSYSERR_NOERROR)
        {
            return buttons;
        }

        ac = SetButtonFromControllerInputs(&buttons, g_Supervisor.m_Cfg.controllerMapping.shotButton, TH_BUTTON_SHOOT,
                                           aa.dwButtons);

        if (g_Supervisor.IsShotSlowEnabled())
        {
            if (ac != 0)
            {
                if (g_FocusButtonConflictState < 20)
                {
                    g_FocusButtonConflictState++;
                }

                if (g_FocusButtonConflictState >= 10)
                {
                    buttons |= TH_BUTTON_FOCUS;
                }
            }
            else
            {
                if (g_FocusButtonConflictState > 10)
                {
                    g_FocusButtonConflictState -= 10;
                    buttons |= TH_BUTTON_FOCUS;
                }
                else
                {
                    g_FocusButtonConflictState = 0;
                }
            }
        }

        SetButtonFromControllerInputs(&buttons, g_Supervisor.m_Cfg.controllerMapping.bombButton, TH_BUTTON_BOMB,
                                      aa.dwButtons);
        SetButtonFromControllerInputs(&buttons, g_Supervisor.m_Cfg.controllerMapping.bombButton, TH_BUTTON_FOCUS,
                                      aa.dwButtons);
        SetButtonFromControllerInputs(&buttons, g_Supervisor.m_Cfg.controllerMapping.menuButton, TH_BUTTON_MENU,
                                      aa.dwButtons);
        SetButtonFromControllerInputs(&buttons, g_Supervisor.m_Cfg.controllerMapping.upButton, TH_BUTTON_UP,
                                      aa.dwButtons);
        SetButtonFromControllerInputs(&buttons, g_Supervisor.m_Cfg.controllerMapping.downButton, TH_BUTTON_DOWN,
                                      aa.dwButtons);
        SetButtonFromControllerInputs(&buttons, g_Supervisor.m_Cfg.controllerMapping.leftButton, TH_BUTTON_LEFT,
                                      aa.dwButtons);
        SetButtonFromControllerInputs(&buttons, g_Supervisor.m_Cfg.controllerMapping.rightButton, TH_BUTTON_RIGHT,
                                      aa.dwButtons);
        SetButtonFromControllerInputs(&buttons, g_Supervisor.m_Cfg.controllerMapping.skipButton, TH_BUTTON_SKIP,
                                      aa.dwButtons);

        ab = ((g_JoystickCaps.wXmax - g_JoystickCaps.wXmin) / 2 / 2);

        buttons |= JOYSTICK_BUTTON_PRESSED(TH_BUTTON_RIGHT, aa.dwXpos,
                                           JOYSTICK_MIDPOINT(g_JoystickCaps.wXmin, g_JoystickCaps.wXmax) + ab);
        buttons |= JOYSTICK_BUTTON_PRESSED(
            TH_BUTTON_LEFT, JOYSTICK_MIDPOINT(g_JoystickCaps.wXmin, g_JoystickCaps.wXmax) - ab, aa.dwXpos);

        ab = ((g_JoystickCaps.wYmax - g_JoystickCaps.wYmin) / 2 / 2);
        buttons |= JOYSTICK_BUTTON_PRESSED(TH_BUTTON_DOWN, aa.dwYpos,
                                           JOYSTICK_MIDPOINT(g_JoystickCaps.wYmin, g_JoystickCaps.wYmax) + ab);
        buttons |= JOYSTICK_BUTTON_PRESSED(
            TH_BUTTON_UP, JOYSTICK_MIDPOINT(g_JoystickCaps.wYmin, g_JoystickCaps.wYmax) - ab, aa.dwYpos);

        return buttons;
    }
    else
    {
        // FIXME: Next if not matching.
        aaa = g_Supervisor.m_Controller->Poll();
        if (FAILED(aaa))
        {
            i32 retryCount = 0;

            utils::DebugPrint("error : DIERR_INPUTLOST\n");
            aaa = g_Supervisor.m_Controller->Acquire();

            while (aaa == DIERR_INPUTLOST)
            {
                aaa = g_Supervisor.m_Controller->Acquire();
                utils::DebugPrint("error : DIERR_INPUTLOST %d\n", retryCount);

                retryCount++;

                if (retryCount >= 400)
                {
                    return buttons;
                }
            }

            return buttons;
        }
        else
        {
            memset(&a0, 0, sizeof(a0));

            aaa = g_Supervisor.m_Controller->GetDeviceState(sizeof(a0), &a0);

            if (FAILED(aaa))
            {
                return buttons;
            }

            a2 = SetButtonFromDirectInputJoystate(&buttons, g_Supervisor.m_Cfg.controllerMapping.shotButton,
                                                  TH_BUTTON_SHOOT, a0.rgbButtons);

            if (g_Supervisor.IsShotSlowEnabled())
            {
                if (a2 != 0)
                {
                    if (g_FocusButtonConflictState < 20)
                    {
                        g_FocusButtonConflictState++;
                    }

                    if (g_FocusButtonConflictState >= 10)
                    {
                        buttons |= TH_BUTTON_FOCUS;
                    }
                }
                else
                {
                    if (g_FocusButtonConflictState > 10)
                    {
                        g_FocusButtonConflictState -= 10;
                        buttons |= 0x4;
                    }
                    else
                    {
                        g_FocusButtonConflictState = 0;
                    }
                }
            }

            SetButtonFromDirectInputJoystate(&buttons, g_Supervisor.m_Cfg.controllerMapping.bombButton, TH_BUTTON_BOMB,
                                             a0.rgbButtons);
            SetButtonFromDirectInputJoystate(&buttons, g_Supervisor.m_Cfg.controllerMapping.bombButton, TH_BUTTON_FOCUS,
                                             a0.rgbButtons);
            SetButtonFromDirectInputJoystate(&buttons, g_Supervisor.m_Cfg.controllerMapping.menuButton, TH_BUTTON_MENU,
                                             a0.rgbButtons);
            SetButtonFromDirectInputJoystate(&buttons, g_Supervisor.m_Cfg.controllerMapping.upButton, TH_BUTTON_UP,
                                             a0.rgbButtons);
            SetButtonFromDirectInputJoystate(&buttons, g_Supervisor.m_Cfg.controllerMapping.downButton, TH_BUTTON_DOWN,
                                             a0.rgbButtons);
            SetButtonFromDirectInputJoystate(&buttons, g_Supervisor.m_Cfg.controllerMapping.leftButton, TH_BUTTON_LEFT,
                                             a0.rgbButtons);
            SetButtonFromDirectInputJoystate(&buttons, g_Supervisor.m_Cfg.controllerMapping.rightButton, TH_BUTTON_RIGHT,
                                             a0.rgbButtons);
            SetButtonFromDirectInputJoystate(&buttons, g_Supervisor.m_Cfg.controllerMapping.skipButton, TH_BUTTON_SKIP,
                                             a0.rgbButtons);

            buttons |= JOYSTICK_BUTTON_PRESSED(TH_BUTTON_RIGHT, a0.lX, g_Supervisor.m_Cfg.padXAxis);
            buttons |= JOYSTICK_BUTTON_PRESSED_INVERT(TH_BUTTON_LEFT, a0.lX, -g_Supervisor.m_Cfg.padXAxis);
            buttons |= JOYSTICK_BUTTON_PRESSED(TH_BUTTON_DOWN, a0.lY, g_Supervisor.m_Cfg.padYAxis);
            buttons |= JOYSTICK_BUTTON_PRESSED_INVERT(TH_BUTTON_UP, a0.lY, -g_Supervisor.m_Cfg.padYAxis);
        }
    }

    return buttons;
}

u32 Controller::SetButtonFromDirectInputJoystate(u16 *outButtons, i16 controllerButtonToTest, u16 touhouButton,
                                                 u8 *inputButtons)
{
    if (controllerButtonToTest < 0)
    {
        return 0;
    }

    *outButtons |= (inputButtons[controllerButtonToTest] & 0x80 ? touhouButton : 0);

    return inputButtons[controllerButtonToTest] & 0x80 ? touhouButton : 0;
}

u32 Controller::SetButtonFromControllerInputs(u16 *outButtons, i16 controllerButtonToTest, u16 touhouButton,
                                              u32 inputButtons)
{
    DWORD mask;

    if (controllerButtonToTest < 0)
    {
        return 0;
    }

    mask = 1 << controllerButtonToTest;

    *outButtons |= (inputButtons & mask ? touhouButton : 0);

    return inputButtons & mask ? touhouButton : 0;
}

DIFFABLE_STATIC_ARRAY(u8, (32 * 4), g_ControllerData)

#pragma var_order(joyinfoex, joyButtonBit, joyButtonIndex, dires, dijoystate2, diRetryCount)
// This is for rebinding keys
u8 *Controller::GetControllerState()
{
    JOYINFOEX joyinfoex;
    u32 joyButtonBit;
    u32 joyButtonIndex;

    i32 dires;
    DIJOYSTATE2 dijoystate2;
    i32 diRetryCount;

    memset(&g_ControllerData, 0, sizeof(g_ControllerData));
    if (g_Supervisor.m_Controller == NULL)
    {
        memset(&joyinfoex, 0, sizeof(JOYINFOEX));
        joyinfoex.dwSize = sizeof(JOYINFOEX);
        joyinfoex.dwFlags = JOY_RETURNALL;
        if (joyGetPosEx(0, &joyinfoex) != JOYERR_NOERROR)
        {
            return g_ControllerData;
        }
        for (joyButtonBit = joyinfoex.dwButtons, joyButtonIndex = 0; joyButtonIndex < 32;
             joyButtonIndex += 1, joyButtonBit >>= 1)
        {
            if ((joyButtonBit & 1) != 0)
            {
                g_ControllerData[joyButtonIndex] = 0x80;
            }
        }
        return g_ControllerData;
    }
    else
    {
        dires = g_Supervisor.m_Controller->Poll();
        if (FAILED(dires))
        {
            diRetryCount = 0;
            utils::DebugPrint("error : DIERR_INPUTLOST\n");
            dires = g_Supervisor.m_Controller->Acquire();
            while (dires == DIERR_INPUTLOST)
            {
                dires = g_Supervisor.m_Controller->Acquire();
                diRetryCount++;
                if (diRetryCount >= 400)
                {
                    utils::DebugPrint("error : DIERR_INPUTLOST %d\n", diRetryCount);
                    return g_ControllerData;
                }
            }
            return g_ControllerData;
        }
        /* dires = */ g_Supervisor.m_Controller->GetDeviceState(sizeof(DIJOYSTATE2), &dijoystate2);
        // TODO: seems ZUN forgot "dires =" above
        if (FAILED(dires))
        {
            return g_ControllerData;
        }
        memcpy(&g_ControllerData, dijoystate2.rgbButtons, sizeof(dijoystate2.rgbButtons));
        return g_ControllerData;
    }
}

u16 Controller::GetInput(void)
{
    u8 keyboardState[256];
    u16 buttons;

    buttons = 0;

    if (g_Supervisor.m_Keyboard == NULL)
    {
        GetKeyboardState(keyboardState);

        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, VK_UP);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, VK_DOWN);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, VK_LEFT);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, VK_RIGHT);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, VK_NUMPAD8);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, VK_NUMPAD2);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, VK_NUMPAD4);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, VK_NUMPAD6);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_LEFT, VK_NUMPAD7);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_RIGHT, VK_NUMPAD9);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_LEFT, VK_NUMPAD1);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_RIGHT, VK_NUMPAD3);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, VK_HOME);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, 'P');
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_D, 'D');
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, 'Z');
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, 'X');
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_FOCUS, VK_SHIFT);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_MENU, VK_ESCAPE);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SKIP, VK_CONTROL);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_Q, 'Q');
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_S, 'S');
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RESET, 'R');
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_ENTER, VK_RETURN);
    }
    else
    {
        HRESULT res = g_Supervisor.m_Keyboard->GetDeviceState(sizeof(keyboardState), keyboardState);

        buttons = 0;

        if (res == DIERR_INPUTLOST)
        {
            g_Supervisor.m_Keyboard->Acquire();

            return Controller::GetControllerInput(buttons);
        }
        if (res != S_OK)
        {
            g_Supervisor.m_Keyboard->Acquire();

            return Controller::GetControllerInput(buttons);
        }

        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, DIK_UP);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, DIK_DOWN);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, DIK_LEFT);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, DIK_RIGHT);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP, DIK_NUMPAD8);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN, DIK_NUMPAD2);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_LEFT, DIK_NUMPAD4);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RIGHT, DIK_NUMPAD6);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_LEFT, DIK_NUMPAD7);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_UP_RIGHT, DIK_NUMPAD9);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_LEFT, DIK_NUMPAD1);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_DOWN_RIGHT, DIK_NUMPAD3);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, DIK_HOME);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_HOME, DIK_P);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_D, DIK_D);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SHOOT, DIK_Z);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_BOMB, DIK_X);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_FOCUS, DIK_LSHIFT);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_FOCUS, DIK_RSHIFT);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_MENU, DIK_ESCAPE);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SKIP, DIK_LCONTROL);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_SKIP, DIK_RCONTROL);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_Q, DIK_Q);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_S, DIK_S);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_ENTER, DIK_RETURN);
        buttons |= KEYBOARD_KEY_PRESSED(TH_BUTTON_RESET, DIK_R);
    }

    return Controller::GetControllerInput(buttons);
}

void Controller::ResetKeyboard(void)
{
    u8 key_states[256];

    GetKeyboardState(key_states);
    for (i32 idx = 0; idx < 256; idx++)
    {
        *(key_states + idx) &= 0x7f;
    }
    SetKeyboardState(key_states);
}

#pragma var_order(inCursor, outCursorBackup, i, out, outCursor, numUnencrypted, unused)
LPBYTE FileSystem::Decrypt(LPBYTE inData, i32 size, u8 xorValue, u8 xorValueInc, i32 chunkSize, i32 maxBytes)
{
    i32 i;
    LPBYTE outCursorBackup;

    i32 unused = 0;
    i32 numUnencrypted = (size % chunkSize < chunkSize / 4) ? size % chunkSize : 0;

    LPBYTE inCursor = inData;
    LPBYTE out = (LPBYTE)g_ZunMemory.Alloc(size, "d:\\cygwin\\home\\zun\\prog\\th08\\global.h");
    LPBYTE outCursor = out;

    if (out == NULL)
    {
        return inData;
    }

    numUnencrypted += size & 1;
    size -= numUnencrypted;

    while (size > 0 && maxBytes > 0)
    {
        if (size < chunkSize)
        {
            chunkSize = size;
        }

        outCursorBackup = outCursor;
        outCursor = &outCursor[chunkSize - 1];

        for (i = (chunkSize + 1) / 2; i > 0; i--, inCursor++)
        {
            *outCursor = *inCursor ^ xorValue;
            outCursor -= 2;
            xorValue += xorValueInc;
        }

        outCursor = &outCursorBackup[chunkSize - 2];

        for (i = chunkSize / 2; i > 0; i--, inCursor++)
        {
            *outCursor = *inCursor ^ xorValue;
            outCursor -= 2;
            xorValue += xorValueInc;
        }

        size -= chunkSize;
        outCursor = &outCursorBackup[chunkSize];
        maxBytes -= chunkSize;
    }

    size += numUnencrypted;
    if (size > 0)
    {
        memcpy(outCursor, inCursor, size);
    }

    return out;
}

struct DecryptParams
{
    u8 key;
    u8 xorValue;
    u8 xorValueInc;
    u8 unused;
    i32 chunkSize;
    i32 maxBytesToDecrypt;
};

DIFFABLE_STATIC_ARRAY_ASSIGN(DecryptParams, 8, g_DecryptParams) = {
    {0x5d, 0x1b, 0x37, 0xaa, 0x0040, 0x2800}, {0x74, 0x51, 0xe9, 0xbb, 0x0040, 0x3000},
    {0x71, 0xc1, 0x51, 0xcc, 0x1400, 0x2000}, {0x8a, 0x03, 0x19, 0xdd, 0x1400, 0x7800},
    {0x95, 0xab, 0xcd, 0xee, 0x0200, 0x1000}, {0xb7, 0x12, 0x34, 0xff, 0x0400, 0x2800},
    {0x9d, 0x35, 0x97, 0x11, 0x0080, 0x2800}, {0xaa, 0x99, 0x37, 0x77, 0x0400, 0x1000},
};

DIFFABLE_STATIC_ARRAY_ASSIGN(u8, 3, g_CryptSignature) = {0x85, 0xa4, 0xda};

#pragma var_order(rawData, decryptedData, i)
LPBYTE FileSystem::TryDecryptFromTable(LPBYTE inData, LPINT unused, i32 size)
{
    LPBYTE rawData = inData;
    LPBYTE decryptedData;

    if (rawData[0] == g_CryptSignature[0] - 0x20 && rawData[1] == g_CryptSignature[1] - 0x40 &&
        rawData[2] == g_CryptSignature[2] - 0x60)
    {
        u32 i = 0;
        while (rawData[3] != g_DecryptParams[i].key - (i << 4) - 0x10 && i < 8)
        {
            i++;
        }
        if (i >= 8)
        {
            return rawData;
        }

        // 4 bytes are skipped to exclude the encryption signature
        decryptedData = Decrypt(rawData + 4, size - 4, g_DecryptParams[i].xorValue, g_DecryptParams[i].xorValueInc,
                                g_DecryptParams[i].chunkSize, g_DecryptParams[i].maxBytesToDecrypt);
        g_ZunMemory.Free(inData);
        return decryptedData;
    }

    return rawData;
}

#pragma var_order(inCursor, i, out, outCursor, numUnencrypted, inCursorBackup, unused)
LPBYTE FileSystem::Encrypt(LPBYTE inData, i32 size, u8 xorValue, u8 xorValueInc, i32 chunkSize, i32 maxBytes)
{
    i32 i;
    LPBYTE inCursorBackup;

    i32 unused = 0;
    i32 numUnencrypted = (size % chunkSize < chunkSize / 4) ? size % chunkSize : 0;

    LPBYTE inCursor = inData;
    LPBYTE out = (LPBYTE)g_ZunMemory.Alloc(size, "d:\\cygwin\\home\\zun\\prog\\th08\\global.h");
    LPBYTE outCursor = out;

    if (out == NULL)
    {
        return inData;
    }

    numUnencrypted += size & 1;
    size -= numUnencrypted;

    while (size > 0 && maxBytes > 0)
    {
        if (size < chunkSize)
        {
            chunkSize = size;
        }

        inCursorBackup = inCursor;
        inCursor = &inCursor[chunkSize - 1];

        for (i = (chunkSize + 1) / 2; i > 0; i--, outCursor++)
        {
            *outCursor = *inCursor ^ xorValue;
            inCursor -= 2;
            xorValue += xorValueInc;
        }

        inCursor = &inCursorBackup[chunkSize - 2];

        for (i = chunkSize / 2; i > 0; i--, outCursor++)
        {
            *outCursor = *inCursor ^ xorValue;
            inCursor -= 2;
            xorValue += xorValueInc;
        }

        size -= chunkSize;
        inCursor = &inCursorBackup[chunkSize];
        maxBytes -= chunkSize;
    }

    size += numUnencrypted;
    if (size > 0)
    {
        memcpy(outCursor, inCursor, size);
    }

    return out;
}

#pragma var_order(unused, entryname, size, data, handle)
LPBYTE FileSystem::OpenFile(LPSTR path, i32 *fileSize, BOOL isExternalResource)
{
    char *entryname;
    DWORD size;
    LPBYTE data;
    HANDLE handle;
    i32 unused = -1;

    g_Supervisor.EnterCriticalSectionWrapper(2);

    if (!isExternalResource)
    {
        entryname = strrchr(path, '\\');
        if (entryname == NULL)
        {
            entryname = path;
        }
        else
        {
            entryname = entryname + 1;
        }
        entryname = strrchr(entryname, '/');
        if (entryname == (char *)0x0)
        {
            entryname = path;
        }
        else
        {
            entryname = entryname + 1;
        }
        size = g_PbgArchive.GetEntryDecompressedSize(entryname);
        if (fileSize != NULL)
        {
            *fileSize = size;
        }
        if (size == 0)
        {
            g_GameErrorContext.Fatal("error : %s is not found in arcfile.\n", entryname);
            goto error;
        }
        if (size != 0)
        {
            utils::DebugPrint("%s Decode ... \n", entryname);

            data = (LPBYTE)g_ZunMemory.Alloc(size, path);
            if (data == NULL)
            {
                goto error;
            }
            g_PbgArchive.ReadDecompressEntry(entryname, data);
            data = TryDecryptFromTable(data, fileSize, size);
            goto done;
        }
    }

    utils::DebugPrint("%s Load ... \n", path);

    handle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,
                         FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {
        utils::DebugPrint("error : %s is not found.\n", path);
        goto error;
    }

    size = GetFileSize(handle, NULL);
    data = (LPBYTE)g_ZunMemory.Alloc(size, path);
    if (data == NULL)
    {
        utils::DebugPrint("error : %s allocation error.\n", path);
        CloseHandle(handle);
        goto error;
    }

    ReadFile(handle, data, size, &size, NULL);
    if (fileSize != NULL)
    {
        *fileSize = size;
    }

    CloseHandle(handle);
    data = TryDecryptFromTable(data, fileSize, size);

done:
    g_Supervisor.LeaveCriticalSectionWrapper(2);
    return data;

error:
    g_Supervisor.LeaveCriticalSectionWrapper(2);
    return NULL;
}

BOOL FileSystem::CheckIfFileAlreadyExists(LPCSTR path)
{
    g_Supervisor.EnterCriticalSectionWrapper(2);

    HANDLE handle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,
                                FILE_FLAG_SEQUENTIAL_SCAN | FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(handle);
        g_Supervisor.LeaveCriticalSectionWrapper(2);
        return TRUE;
    }

    g_Supervisor.LeaveCriticalSectionWrapper(2);
    return FALSE;
}

#pragma var_order(numBytesWritten, handle, buffer)
int FileSystem::WriteDataToFile(LPCSTR path, LPVOID data, size_t size)
{
    LPSTR buffer;
    DWORD numBytesWritten;

    g_Supervisor.EnterCriticalSectionWrapper(2);

    HANDLE handle = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle == INVALID_HANDLE_VALUE)
    {
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                       NULL, GetLastError(), LANG_USER_DEFAULT, (LPSTR)&buffer, 0, NULL);

        utils::DebugPrint("error : %s write error %s\n", path, buffer);
        LocalFree(buffer);
        g_Supervisor.LeaveCriticalSectionWrapper(2);
        return -1;
    }

    WriteFile(handle, data, size, &numBytesWritten, NULL);
    if (size != numBytesWritten)
    {
        CloseHandle(handle);
        utils::DebugPrint("error : %s write error\n", path);
        g_Supervisor.LeaveCriticalSectionWrapper(2);
        return -2;
    }

    CloseHandle(handle);
    utils::DebugPrint("%s write ...\n", path);
    g_Supervisor.LeaveCriticalSectionWrapper(2);
    return 0;
}

const char *GameErrorContext::Log(const char *fmt, ...)
{
    char tmpBuffer[0x2000];
    size_t tmpBufferSize;
    va_list args;

    va_start(args, fmt);
    g_Supervisor.EnterCriticalSectionWrapper(3);
    vsprintf(tmpBuffer, fmt, args);

    tmpBufferSize = strlen(tmpBuffer);

    if (m_BufferEnd + tmpBufferSize < &m_Buffer[sizeof(m_Buffer) - 1])
    {
        strcpy(m_BufferEnd, tmpBuffer);

        m_BufferEnd += tmpBufferSize;
        m_BufferEnd[0] = '\0';
    }

    va_end(args);

    g_Supervisor.LeaveCriticalSectionWrapper(3);
    return fmt;
}

const char *GameErrorContext::Fatal(const char *fmt, ...)
{
    char tmpBuffer[512];
    size_t tmpBufferSize;
    va_list args;

    va_start(args, fmt);
    g_Supervisor.EnterCriticalSectionWrapper(3);
    vsprintf(tmpBuffer, fmt, args);

    tmpBufferSize = strlen(tmpBuffer);

    if (m_BufferEnd + tmpBufferSize < &m_Buffer[sizeof(m_Buffer) - 1])
    {
        strcpy(m_BufferEnd, tmpBuffer);

        m_BufferEnd += tmpBufferSize;
        m_BufferEnd[0] = '\0';
    }

    va_end(args);

    m_ShowMessageBox = true;

    g_Supervisor.LeaveCriticalSectionWrapper(3);
    return fmt;
}

u16 Rng::GetRandomU16(void)
{
    u16 temp = (m_Seed ^ 0x9630) - 0x6553;
    m_Seed = (((temp & 0xc000) >> 14) + temp * 4) & 0xffff;
    m_GenerationCount++;
    return m_Seed;
}

u32 Rng::GetRandomU32(void)
{
    return GetRandomU16() << 16 | GetRandomU16();
}

f32 Rng::GetRandomF32(void)
{
    // XXX: Divisor is rounded is rounded to UINT_MAX+1 because of floating point
    // jank
    return (f32)GetRandomU32() / (f32)UINT_MAX;
}

f32 Rng::GetRandomF32Signed(void)
{
    // XXX: Divisor is rounded is rounded to INT_MAX+1 because of floating point
    // jank
    return (f32)GetRandomU32() / (f32)INT_MAX - 1.0f;
}

ZunMemory::ZunMemory()
{
    m_bRegistryInUse = FALSE;
}

ZunMemory::~ZunMemory()
{
    if (m_bRegistryInUse)
    {
        for (i32 i = 0; i < ARRAY_SIZE_SIGNED(m_Registry); i++)
        {
            if (m_Registry[i] != NULL)
            {
                free(m_Registry[i]);
            }
        }
    }
}

f32 AddNormalizeAngle(f32 a, f32 b)
{
    i32 i;

    i = 0;
    a += b;
    while (a > ZUN_PI)
    {
        a -= ZUN_2PI;
        if (i++ > 16)
            break;
    }
    while (a < -ZUN_PI)
    {
        a += ZUN_2PI;
        if (i++ > 16)
            break;
    }
    return a;
}

#pragma var_order(sinOut, cosOut)
void Rotate(D3DXVECTOR3 *outVector, D3DXVECTOR3 *point, f32 angle)
{
    f32 sinOut = sinf(angle);
    f32 cosOut = cosf(angle);
    outVector->x = cosOut * point->x - sinOut * point->y;
    outVector->y = cosOut * point->y + sinOut * point->x;
}

GameErrorContext::GameErrorContext()
{
    m_BufferEnd = m_Buffer;
    m_BufferEnd[0] = '\0';
    m_ShowMessageBox = false;
}

GameErrorContext::~GameErrorContext()
{
}
}; // namespace th08
