#include <kernel/kernel.h>

#include <kernel/common/log.h>
#include <kernel/kutil.h>

framebuffer_t kernel_fb;

void
_start(void)
{
  kinit();
  kprint("a\e[32mHello,\e[34m Worl\e[0md!\n");

  //kprintf("%d", 10 / 0);

  for (;;) {
    __asm__("hlt");
  }
}
