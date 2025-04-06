#pragma once

#include <windows.h>
#include "diffbuild.hpp"
#include "inttypes.hpp"
#include "utils.hpp"

namespace th08
{
struct Th8k
{
    u32 magic;
    u16 th8kLen;
    u16 unkLen;
    u8 version;
    unknown_fields(0x9, 0x3);
};
C_ASSERT(sizeof(Th8k) == 0xC);

struct PlstPlayCounts
{
    i32 attemptsTotal;
    i32 attemptsPerCharacter[12];
    unknown_fields(0x34, 0x4);
    i32 clears;
    i32 continues;
    i32 practices;
};
C_ASSERT(sizeof(PlstPlayCounts) == 0x44);

struct Plst
{
    Th8k base;
    i32 totalHours;
    i32 totalMinutes;
    i32 totalSeconds;
    unknown_fields(0x18, 0x4);
    i32 gamehours;
    i32 gameminutes;
    i32 gameSeconds;
    unknown_fields(0x28, 0x4);
    PlstPlayCounts playDataByDifficulty[6];
    PlstPlayCounts playDataTotals;
    u8 bgmUnlocked[21];
};
C_ASSERT(sizeof(Plst) == 0x220);

struct GameManager
{
    unknown_fields(0x0, 0x3d804);
    Plst plst;

    unknown_fields(0x3da21, 0x18e);

    u8 m_IsInGameMenu;
    unknown_fields(0x3dbb3, 0x289);
};
C_ASSERT(sizeof(GameManager) == 0x3de3c);

DIFFABLE_EXTERN(GameManager, g_GameManager);
}; // Namespace th08

