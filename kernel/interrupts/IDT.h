#ifndef KERNEL_IDT_H
#define KERNEL_IDT_H

#include <kernel/kernel.h>

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

#if defined(__TINYC__)
#pragma pack(1)
#endif

typedef struct PACKED {
  u16 limit; // Size.
  u64 base;  // Base address.
} idt_ptr_t;

#if defined(__TINYC__)
#pragma pack(1)
#endif

void idt_set_descriptor(u8 vector, void *isr, u8 flags);
void idt_install(void);

#endif // KERNEL_IDT_H
