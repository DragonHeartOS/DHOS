#pragma once

#include <commonlib/types.h>

#include <kernel/drivers/video/framebuffer/fb.h>

#define PACKED __attribute__((packed))

extern framebuffer_t kernel_fb;
