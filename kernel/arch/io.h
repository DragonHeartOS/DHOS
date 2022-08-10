#pragma once

#include <commonlib/types.h>

inline static void
outb(u16 port, u8 val)
{
  asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

inline static u8
inb(u16 port)
{
  u8 ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

inline static void
wait(void)
{
  // FIXME: This is probably fragile.
  asm volatile("jmp 1f\n\t"
               "1:jmp 2f\n\t"
               "2:");
}
