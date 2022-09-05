#pragma once

#include <commonlib/types.h>

typedef struct {
  u8 r, g, b, a;
} rgba_t;

#define COLOR_WHITE   ((rgba_t){ 255, 255, 255, 255 })
#define COLOR_BLACK   ((rgba_t){   0,   0,   0, 255 })

#define COLOR_RED     ((rgba_t){ 255,   0,   0, 255 })
#define COLOR_GREEN   ((rgba_t){   0, 255,   0, 255 })
#define COLOR_YELLOW  ((rgba_t){ 255, 255,   0, 255 })
#define COLOR_BLUE    ((rgba_t){   0,   0, 255, 255 })
#define COLOR_MAGENTA ((rgba_t){ 255,   0, 255, 255 })
#define COLOR_CYAN    ((rgba_t){   0, 255, 255, 255 })

#define COLOR_DHRED   ((rgba_t){ 127,   0,   0, 255 })

