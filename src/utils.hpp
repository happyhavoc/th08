#pragma once

#include "inttypes.hpp"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define ARRAY_SIZE_SIGNED(x) ((i32)sizeof(x) / (i32)sizeof(x[0]))

namespace th08
{
namespace utils
{
void DebugPrint(char *fmt, ...);
void GuiDebugPrint(char *fmt, ...);
}; // namespace utils
}; // namespace th08
