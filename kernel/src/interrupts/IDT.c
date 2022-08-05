#include <interrupts/IDT.h>

static interrupt_gate_descriptor_t idt[256];
static idt_ptr_t idt_ptr;

void
idt_set_descriptor(u8 vector, void *isr, u8 flags)
{
  u64 addr = (u64)isr;
  interrupt_gate_descriptor_t *vec = &idt[vector];

  vec->isr_low16 = addr & 0xFF;
  vec->isr_mid16 = (addr >> 16) & 0xFF;
  vec->isr_high32 = addr >> 32;
  vec->ist = 0;
  vec->zero = 0;
  vec->zero1 = 0;
  vec->zero2 = 0;
  vec->reserved = 0;
  vec->p = 1;
  vec->attr = flags;
  vec->kernel_cs = 0x28;
}

void
idt_install(void)
{
  idt_ptr.limit = sizeof(interrupt_gate_descriptor_t)*256 - 1;
  idt_ptr.base = (u64)&idt;
  __asm__ __volatile__("lidt %0" :: "m" (idt_ptr));
}

