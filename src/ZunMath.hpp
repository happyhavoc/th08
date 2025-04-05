#pragma once
#include <math.h>
#include "inttypes.hpp"

#define ZUN_PI (3.14159265358979323846f)

// Redundant typecast required for matching assembly
#define ZUN_TAN(X) ((f32) tan(X))
