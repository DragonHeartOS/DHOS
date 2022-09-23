#include <kernel/kpanic.h>

#include <kernel/common/log.h>
#include <kernel/kernel.h>

#include <commonlib/color.h>

void
kpanic(char *const msg)
{
  fb_cursor = (vec2_t){0, 0};
  fb_fg = COLOR_WHITE;
  fb_bg = COLOR_RED;

  kprint("KERNEL PANIC\n------------\n\n");

  kprint(msg);
  kprint("\n");

  ASMV("cli");
  for (;;)
    __asm__("hlt");
}
