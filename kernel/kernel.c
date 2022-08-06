#include <common/log.h>
#include <limine.h>
#include <stddef.h>
#include <stdint.h>

void
_start(void)
{
  kprintf(KINFO "Hello, World!\n");

  for (;;) {
    __asm__("hlt");
  }
}
