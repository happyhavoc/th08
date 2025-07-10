#pragma once
#include <windows.h>
#include "diffbuild.hpp"
#include "inttypes.hpp"
#include "utils.hpp"

namespace th08
{
struct Background
{
    unknown_fields(0x0, 0xb20);
    u8 m_SkyFogNeedsSetup; // Leftover from earlier games. Never checked in IN
    unknown_fields(0xb21, 0x5adf);
};
C_ASSERT(sizeof(Background) == 0x6600);

DIFFABLE_EXTERN(Background, g_Background);
}; // Namespace th08
