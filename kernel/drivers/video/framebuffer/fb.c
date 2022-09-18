#include <kernel/drivers/video/framebuffer/fb.h>

#include <commonlib/math.h>
#include <kernel/common/string.h>
#include <kernel/drivers/video/framebuffer/font.h>

font_t *cfont = (font_t *)&DEFAULT_FONT;

vec2_t fb_cursor = {0, 0};

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
    fb_cursor.x = 0;

    if (fb_cursor.y + cfont->height > fb->height) {
      // fb_scroll_down();
    } else {
      fb_cursor.y += 1;
    }
  } else {
    fb_text_putch_raw(fb, (u16)(fb_cursor.x * cfont->width),
                      (u16)(fb_cursor.y * cfont->height), ch, color, bgcolor);

    fb_cursor.x += 1;

    if (fb_cursor.x > fb->width) {
      fb_cursor.x = 0;
      fb_cursor.y += 1;
    }
  }
}

void
fb_text_putstr(framebuffer_t *fb, char *str, u64 len, rgba_t color,
               rgba_t bgcolor)
{
  u64 i;
  for (i = 0; i < len; i++)
    fb_text_putch(fb, str[i], color, bgcolor);
}

rgba_t
select_color_from_id(u8 id)
{
  rgba_t ret;
  switch (id) {
  case 1: ret = COLOR_RED; break;
  case 2: ret = COLOR_GREEN; break;
  case 3: ret = COLOR_YELLOW; break;
  case 4: ret = COLOR_BLUE; break;
  case 5: ret = COLOR_MAGENTA; break;
  case 6: ret = COLOR_CYAN; break;
  case 7: ret = COLOR_WHITE; break;
  case 9:
  case 0:
  default: ret = COLOR_BLACK; break;
  }
  return ret;
}

rgba_t fb_fg = COLOR_WHITE;
rgba_t fb_bg = COLOR_BLACK;

void
fb_text_putstr_ex(framebuffer_t *fb, char const *str)
{
  // FIXME: Add '\0' checks
  while (*str != '\0') {
    if (*str == '\e') {
      str++;

      if (*str == '[') {
        str++;

        switch (*str) {
        case '3': {
          str++;
          fb_fg = select_color_from_id((u8)*str - '0');
          break;
        }
        case '4': {
          str++;
          fb_bg = select_color_from_id((u8)*str - '0');
          break;
        }
        case '0':
          fb_fg = COLOR_WHITE;
          fb_bg = COLOR_BLACK;
          break;
        }
        str += 2;
      }
    }

    fb_text_putch(fb, *str, fb_fg, fb_bg);
    str++;
  }
}
