#pragma once

#include "inttypes.hpp"

#define COLOR_BLACK 0xff000000
#define COLOR_LIGHT_GREY 0xffa0a0a0

#define COLOR_RGB_MASK 0x00FFFFFF
#define COLOR_ALPHA_MASK 0xFF000000
#define COLOR_RGB(color) ((color) & COLOR_RGB_MASK)
#define COLOR_ALPHA(color) (((color) & COLOR_ALPHA_MASK) >> 24)
#define COLOR_SET_ALPHA(color, alpha) (((alpha) << 24) | COLOR_RGB(color))
#define COLOR_SET_ALPHA2(color, alpha) (COLOR_RGB(color) | (((alpha) & 0xff) << 24))
#define COLOR_SET_ALPHA3(color, alpha) (COLOR_RGB(color) | ((alpha) << 24))
#define COLOR_COMBINE_ALPHA(color, alpha) (((alpha) & COLOR_ALPHA_MASK) | COLOR_RGB(color))

// TODO: The following assumes little endian
#define COLOR_RED_BYTE_IDX 0
#define COLOR_GREEN_BYTE_IDX 1
#define COLOR_BLUE_BYTE_IDX 2
#define COLOR_ALPHA_BYTE_IDX 3

#define COLOR_GET_COMPONENT(color, component) (((u8 *)&(color))[(component)])
#define COLOR_SET_COMPONENT(color, component, value) ((u8 *)&(color))[(component)] = (value);

typedef u32 ZunColor;
