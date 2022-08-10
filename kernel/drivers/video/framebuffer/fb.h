#pragma once

#include <commonlib/color.h>
#include <commonlib/types.h>

typedef struct {
  u64 addr;
  u16 width, height, pitch, bpp;
  u8 *data;
} framebuffer_t;

void fb_pixel_put(framebuffer_t *fb, u16 x, u16 y, rgba_t color);
void fb_clear_screen(framebuffer_t *fb, rgba_t color);

void fb_text_putch_raw(framebuffer_t *fb, u16 x, u16 y, char ch, rgba_t color,
                       rgba_t bgcolor);
void fb_text_putch(framebuffer_t *fb, char ch, rgba_t color, rgba_t bgcolor);
void fb_text_putstr(framebuffer_t *fb, char *str, u64 len, rgba_t color,
                    rgba_t bgcolor);
