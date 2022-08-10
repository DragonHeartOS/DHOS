#include <kernel/kernel.h>

#include <kernel/kutil.h>
#include <common/log.h>

void
_start(void)
{
  kinit();
  kprintf(KINFO "Hello, World!\n");

  for (;;) {
    __asm__("hlt");
  }
}
