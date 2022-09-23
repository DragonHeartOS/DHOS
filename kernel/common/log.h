#ifndef KERNEL_LOG_H
#define KERNEL_LOG_H

#include <kernel/kernel.h>

#define KINFO "\e[32m"
#define KCLEAR "\e[0m"

void kprint(char *str);
void kprintf(char *fmt, ...);
void clear_term(void);

#endif // KERNEL_LOG_H
