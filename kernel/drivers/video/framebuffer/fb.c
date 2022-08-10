#include <kernel/drivers/video/framebuffer/fb.h>

#include <commonlib/math.h>
#include <kernel/drivers/video/framebuffer/font.h>

font_t *cfont = (font_t *)&DEFAULT_FONT;

vec2_t cursor = {0, 0};

void
fb_pixel_put(framebuffer_t *fb, u16 x, u16 y, rgba_t color)
{
  u8 *base = fb->data + x * 4 + y * fb->pitch;

  base[0] = color.b;
  base[1] = color.g;
  base[2] = color.r;
  base[3] = color.a;
}

void
fb_clear_screen(framebuffer_t *fb, rgba_t color)
{
  u16 x, y;
  for (y = 0; y < fb->height; y++)
    for (x = 0; x < fb->width; x++)
      fb_pixel_put(fb, x, y, color);
}

void
fb_text_putch_raw(framebuffer_t *fb, u16 x, u16 y, char ch, rgba_t color,
                  rgba_t bgcolor)
{
  u16 tmp_x, tmp_y;

  ch -= 32;
  for (tmp_x = 0; tmp_x < cfont->width; tmp_x++) {
    for (tmp_y = 0; tmp_y < cfont->height; tmp_y++) {
      u16 col = cfont->data[(u64)ch * cfont->width + tmp_x];
      col = (col >> tmp_y) & 1;
      fb_pixel_put(fb, x + tmp_x, y + tmp_y, col ? color : bgcolor);
    }
  }
}

void
fb_text_putch(framebuffer_t *fb, char ch, rgba_t color, rgba_t bgcolor)
{
  if (ch == '\n') {
    cursor.x = 0;

    if (cursor.y + cfont->height > fb->height) {
      // fb_scroll_down();
    } else {
      cursor.y += 1;
    }
  } else {
    fb_text_putch_raw(fb, (u16)(cursor.x * cfont->width),
                      (u16)(cursor.y * cfont->height), ch, color, bgcolor);

    cursor.x += 1;

    if (cursor.x > fb->width) {
      cursor.x = 0;
      cursor.y += 1;
    }
  }
}

void
fb_text_putstr(framebuffer_t *fb, char *str, u64 len, rgba_t color,
               rgba_t bgcolor)
{
  u64 i = 0;
  for (i = 0; i < len; i++)
    fb_text_putch(fb, str[i], color, bgcolor);
}
