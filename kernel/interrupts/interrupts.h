#pragma once

void exception_divide_error(void);
void exception_debug_exception(void);
void exception_overflow(void);
void exception_bound_range_exceeded(void);
void exception_invalid_opcode(void);
void exception_no_mathcoprocessor(void);
void exception_double_fault(void);
void exception_invalid_tss(void);
void exception_segment_not_present(void);
void exception_stack_segment_fault(void);
void exception_general_protection_fault(void);
void exception_page_fault(void);

void interrupts_init(void);
void irq_init(void);
