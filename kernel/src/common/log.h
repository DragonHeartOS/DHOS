#pragma once

#include <kernel.h>

#define KINFO "\e[0;32m"
#define BLUE "\e[0;34m"
#define YELLOW "\e[0;33m"
#define PURPLE "\e[0;35m"
#define CYAN "\e[0;36m"
#define WHITE "\e[0;37m"
#define BOLD_YELLOW "\e[1;33m"

void kprintf(char* fmt, ...);
void clear_term(void);

