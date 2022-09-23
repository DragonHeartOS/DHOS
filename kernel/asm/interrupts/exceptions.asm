bits 64

extern kpanic

global exception_divide_error
global exception_debug_exception
global exception_overflow
global exception_bound_range_exceeded
global exception_invalid_opcode
global exception_no_mathcoprocessor
global exception_double_fault
global exception_invalid_tss
global exception_segment_not_present
global exception_stack_segment_fault
global exception_general_protection_fault
global exception_page_fault

%macro kpanic 1
        cli
        mov rdi, panic_msg
        call kpanic
        hlt
%endmacro

exception_divide_error:
        kpanic 0x0

exception_debug_exception:
        kpanic 0x1

exception_overflow:
        kpanic 0x4

exception_bound_range_exceeded:
        kpanic 0x5

exception_invalid_opcode:
        kpanic 0x6

exception_no_mathcoprocessor:
        kpanic 0x7

exception_double_fault:
        kpanic 0x8

exception_invalid_tss:
        kpanic 0xA

exception_segment_not_present:
        kpanic 0xB

exception_stack_segment_fault:
        kpanic 0xC

exception_general_protection_fault:
        kpanic 0xD

exception_page_fault:
        kpanic 0xE


section .data
panic_msg:      db "INTERRUPT FIRED", 0
        
