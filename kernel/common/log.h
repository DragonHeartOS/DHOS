#pragma once

#include <kernel.h>

#define KINFO "\e[0;32m"
#define KCLEAR "\e[0;0m"

void kprintf(char *fmt, ...);
void clear_term(void);
