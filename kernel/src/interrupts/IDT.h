#pragma once

#include <kernel.h>

typedef struct {
  u16 isr_low16;
  u16 kernel_cs;
  u8 ist : 2;
  u8 zero : 1;
  u8 zero1 : 3;
  u8 attr : 4;
  u8 zero2 : 1;
  u8 dpl : 2;
  u8 p : 1;
  u16 isr_mid16;
  u32 isr_high32;
  u32 reserved;
} interrupt_gate_descriptor_t;

typedef struct {
  u16 limit; // Size.
  u64 base;  // Base address.
} __attribute__((packed)) idt_ptr_t;

void idt_set_descriptor(u8 vector, void *isr, u8 flags);
void idt_install(void);
