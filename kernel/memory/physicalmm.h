#ifndef KERNEL_PHYSICALMM_H
#define KERNEL_PHYSICALMM_H

#include <commonlib/types.h>

u64 physicalmm_alloc(void);
void physicalmm_free(u64 addr);
void physicalmm_init(void);

#endif // KERNEL_PHYSICALMM_H
