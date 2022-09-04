#pragma once

#include <kernel/kernel.h>

size_t strlen(char const *str);
u8 memcmp(char const *str1, char const *str2, size_t n);
char *dec2str(int dec);
u8 *strncpy(u8 *dst, u8 const *src, u8 const len);
u8 *hex2str(u32 hex_num);
void memzero(void *ptr, size_t n);
void memset(void *ptr, u64 data, size_t n);
u8 strcmp(char const *str1, char const *str2);
u8 strncmp(char const *str1, char const *str2, size_t n);
void *memcpy(void *dest, void const *src, size_t n);
void *memmove(void *dest, void const *src, size_t n);
