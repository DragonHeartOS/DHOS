#ifndef KERNEL_H
#define KERNEL_H

#include <commonlib/types.h>

#include <kernel/drivers/video/framebuffer/fb.h>

#define PACKED __attribute__((packed))

extern framebuffer_t kernel_fb;

#endif // KERNEL_H
