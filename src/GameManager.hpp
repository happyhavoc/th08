#pragma once

#include <windows.h>
#include "diffbuild.hpp"
#include "inttypes.hpp"
#include "utils.hpp"

namespace th08
{
struct GameManager
{
    unknown_fields(0x0, 0x3dbb2);
    u8 m_IsInGameMenu;
    unknown_fields(0x3dbb3, 0x289);
};
C_ASSERT(sizeof(GameManager) == 0x3de3c);

DIFFABLE_EXTERN(GameManager, g_GameManager);
}; // Namespace th08

