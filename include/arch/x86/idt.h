#pragma once

#include <stdint.h>

typedef struct {
    uint16_t offset_1;
    uint16_t selector;
    uint8_t  ist;
    uint8_t  attrs;
    uint16_t offset_2;
    uint32_t offset_3;
    uint32_t zero;
} idt_entry;

typedef struct {
    uint16_t size;
    uint64_t* offset;
} __attribute__((packed)) idt_descriptor;

typedef void (*idt_handler_function)(void);

void x86_idt_init(void);
void x86_idt_set_descriptor(uint8_t vector, idt_handler_function isr, uint8_t flags);
