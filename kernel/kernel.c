#include "drivers/comm/serial/serial.h"
#include "drivers/video/framebuffer/fb.h"
#include <kernel/kernel.h>

#include <common/log.h>

#include <kernel/limine.h>

static volatile struct limine_framebuffer_request fb_req = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

static struct limine_framebuffer *fb_limine = NULL;

framebuffer_t kernel_fb_stat;
framebuffer_t *kernel_fb = &kernel_fb_stat;

void
kinit()
{
  if (fb_limine == NULL) {
    fb_limine = fb_req.response->framebuffers[0];
  }
  kernel_fb_stat = (framebuffer_t){.addr = (u64)fb_limine->address,
                                   .width = (u16)fb_limine->width,
                                   .height = (u16)fb_limine->height,
                                   .pitch = (u16)fb_limine->pitch,
                                   .bpp = fb_limine->bpp,
                                   .data = fb_limine->address};

  serial_init();
}

void
_start(void)
{
  kinit();
  kprintf(KINFO "Hello, World!\n");

  for (;;) {
    __asm__("hlt");
  }
}
