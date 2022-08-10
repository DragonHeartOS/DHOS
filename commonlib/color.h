#pragma once

#include <commonlib/types.h>

typedef struct {
  u8 r, g, b, a;
} rgba_t;

#define COLOR_WHITE ((rgba_t){255,255,255,255})
#define COLOR_BLACK ((rgba_t){0,0,0,0})

