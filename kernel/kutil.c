#include <kernel/kutil.h>

#include <kernel/limine.h>

#include <kernel/drivers/comm/serial/serial.h>
#include <kernel/drivers/video/framebuffer/fb.h>

static volatile struct limine_framebuffer_request fb_req = {
    .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = 0};

static struct limine_framebuffer *fb_limine = NULL;

u8 kernel_initilized = false;

void
kinit(void)
{
  if (fb_limine == NULL) {
    fb_limine = fb_req.response->framebuffers[0];
  }

  kernel_fb = (framebuffer_t){.addr = (u64)fb_limine->address,
                              .width = (u16)fb_limine->width,
                              .height = (u16)fb_limine->height,
                              .pitch = (u16)fb_limine->pitch,
                              .bpp = fb_limine->bpp,
                              .data = fb_limine->address};

  serial_init();

  kernel_initilized = true;
}


