#include <kernel/interrupts/IDT.h>

static interrupt_gate_descriptor_t idt[256];
static idt_ptr_t idt_ptr;

void
idt_set_descriptor(u8 vector, void *isr, U8 flags)
{
  u64 addr = (u64)isr;
  interrupt_gate_descriptor_t *vec = &idt[vector];

  vec->isr_low16 = addr & 0xFFFF;
  vec->kernel_cs = 0x28;
  vec->ist = 0;
  vec->zero = 0;
  vec->zero1 = 0;
  vec->attr = flags.hi;
  vec->zero2 = 0;
  vec->dpl = 3;
  vec->p = 1;
  vec->isr_mid16 = (addr >> 16) & 0xFFFF;
  vec->isr_high32 = (addr >> 32);
  vec->reserved = 0;
}

void
idt_install(void)
{
  idt_ptr.limit = sizeof(interrupt_gate_descriptor_t) * 256 - 1;
  idt_ptr.base = (u64)&idt;
  __asm__ __volatile__("lidt %0" ::"m"(idt_ptr));
}
