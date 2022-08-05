#pragma once

#include <stdint.h>

typedef struct {
    uint16_t isr_low16;
    uint16_t kernel_cs;
    uint8_t ist : 2;
    uint8_t zero : 1;
    uint8_t zero1 : 3;
    uint8_t attr : 4;
    uint8_t zero2 : 1;
    uint8_t dpl : 2;
    uint8_t p : 1;
    uint16_t isr_mid16;
    uint32_t isr_high32;
    uint32_t reserved;
} interrupt_gate_descriptor_t;


typedef struct {
    uint16_t limit;             // Size.
    uint64_t base;              // Base address.
} __attribute__((packed)) idt_ptr_t;


void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags);
void idt_install(void);
