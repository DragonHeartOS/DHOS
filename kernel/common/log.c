#include <common/log.h>
#include <common/string.h>
#include <limine.h>

#include <kernel/drivers/comm/serial/serial.h>
#include <kernel/drivers/video/framebuffer/fb.h>
#include <kernel/kernel.h>

#include <stdarg.h>

static void
kprint(char *str)
{
  // FIXME: Implement colors
  fb_text_putstr(&kernel_fb, str, strlen(str), COLOR_WHITE, COLOR_BLACK);
  serial_putstr(str);
}

void
kprintf(char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  char *ptr;

  if (memcmp(fmt, KINFO, strlen(KINFO))) {
    kprint(KINFO);
    fmt += strlen(KINFO);
  }

  for (ptr = fmt; *ptr != '\0'; ++ptr) {
    if (*ptr == '%') {
      ++ptr;
      switch (*ptr) {
      case 's': kprint(va_arg(ap, char *)); break;
      case 'd': kprint(dec2str((int)va_arg(ap, u64))); break;
      case 'x': kprint((char *)hex2str((u32)va_arg(ap, u64))); break;
      }
    } else {
      char terminated[2] = {*ptr, 0};
      kprint(terminated);
    }
  }
}

void
clear_term(void)
{
  kprintf("\033[H\033[2J\033[3J");
}
