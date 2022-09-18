#include <kernel/interrupts/interrupts.h>

#include <kernel/interrupts/IDT.h>

#define TRAP_GATE_FLAGS 0x8F

void static (*exceptions[])(void) = {
    exception_divide_error,
    exception_debug_exception,
    exception_general_protection_fault,
    exception_general_protection_fault,
    exception_overflow,
    exception_bound_range_exceeded,
    exception_invalid_opcode,
    exception_no_mathcoprocessor,
    exception_double_fault,
    exception_invalid_tss,
    exception_segment_not_present,
    exception_stack_segment_fault,
    exception_general_protection_fault,
    exception_page_fault,
};

void
exceptions_init(void)
{
  u8 i;
  for (i = 0; i < 14; i++)
    idt_set_descriptor(i, exceptions[i], TRAP_GATE_FLAGS);
}

void
interrupts_init(void)
{
  exceptions_init();
  idt_install();
}

void
interrupts_irq_init(void)
{
}
