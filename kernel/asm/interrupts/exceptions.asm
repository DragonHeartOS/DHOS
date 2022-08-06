bits 64
global divide_error
global debug_exception
global overflow
global bound_range_exceeded
global invalid_opcode
global no_mathcoprocessor
global double_fault
global invalid_tss
global segment_not_present
global stack_segment_fault
global general_protection_fault
global page_fault

%macro kpanic 1
    cli
    hlt
%endmacro

divide_error:
    kpanic 0x0

debug_exception:
    kpanic 0x1

overflow:
    kpanic 0x4

bound_range_exceeded:
    kpanic 0x5

invalid_opcode:
    kpanic 0x6

no_mathcoprocessor:
    kpanic 0x7

double_fault:
    kpanic 0x8

invalid_tss:
    kpanic 0xA

segment_not_present:
    kpanic 0xB

stack_segment_fault:
    kpanic 0xC

general_protection_fault:
    kpanic 0xD

page_fault:
    kpanic 0xE

