#include <arch/x86/idt.h>
#include <arch/x86/io.h>
#include <stdio.h>

static idt_entry idt_table[256];


static void exception_handler(void){
    kputs("pernel kanic!");
    __asm__ volatile ("cli; hlt");
}

void x86_idt_init(void){
    idt_descriptor idtr;
    idtr.size = sizeof(idt_entry) * 256 - 1;
    idtr.offset = (uint64_t*)&idt_table[0];

    for (uint8_t vector = 0x0; vector < 0x20; vector++) {
        x86_idt_set_descriptor(vector, exception_handler, 0x8E);
    }

    __asm__ volatile ("lidt %0; sti" :: "m"(idtr));

    return;
}



void x86_idt_set_descriptor(uint8_t vector, idt_handler_function isr, uint8_t flags){
    idt_entry* entry = &idt_table[vector];

    entry->offset_1 = ((uintptr_t)isr) & 0xffff;
    entry->offset_2 = ((uintptr_t)isr) >> 16;
    entry->offset_3 = ((uintptr_t)isr) >> 32;
    entry->selector = 0x08;
    entry->attrs = flags;
    entry->ist = 0;
    entry->zero = 0;
}
