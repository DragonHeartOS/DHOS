#include <kernel/kernel.h>

#include <kernel/kutil.h>
#include <common/log.h>

framebuffer_t kernel_fb;

void
_start(void)
{
  kinit();
  kprint("a\e[32mHello,\e[34m Worl\e[0md!\n");
  kprintf(KINFO "Hello world!");

  for (;;) {
    __asm__("hlt");
  }
}
