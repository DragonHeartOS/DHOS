#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include <common/log.h>

void
_start(void) {
  kprintf(KINFO "Hello, World!\n");
  
  for (;;) {
    __asm__("hlt");
  }
}
