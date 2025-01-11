#include <arch/x86/idt.h>
#include <arch/x86/io.h>
#include <arch/x86/pic.h>
#include <stdio.h>

static idt_entry idt_table[256];

void exception_handler(void) {
    uint8_t isr = x86_pic_get_isr(0);
    uint8_t isr2 = x86_pic_get_isr(1);
    if (isr == 0xff && isr2 == 0xff) {
        kputs("kernel panic!");
        __asm__ volatile ("cli; hlt");
    }
    x86_pic_isr_handler();
}

extern void exception_handler_asm(void);
__asm__ (
    "exception_handler_asm:"
    "   pushq %rax;"
    "   call exception_handler;"
    "   popq %rax;"
    "   iretq;"
);

void x86_idt_init(void) {
    idt_descriptor idtr;
    idtr.size = sizeof(idt_entry) * 256 - 1;
    idtr.offset = (uint64_t*)&idt_table[0];

    for (uint8_t vector = 0x0; vector < 0x20; vector++) {
        x86_idt_set_descriptor(vector, exception_handler_asm, 0x8E);
    }

    __asm__ volatile ("lidt %0; sti" :: "m"(idtr));
}

void x86_idt_set_descriptor(uint8_t vector, idt_handler_function isr, uint8_t flags) {
    idt_entry* entry = &idt_table[vector];

    entry->offset_1 = ((uintptr_t)isr) & 0xffff; // младшие 16 бит
    entry->offset_2 = ((uintptr_t)isr) >> 16;    // следующие 16 бит
    entry->offset_3 = ((uintptr_t)isr) >> 32;    // старшие 32 бита
    entry->selector = 0x08;                       // сегмент кода
    entry->attrs = flags;                         // атрибуты
    entry->ist = 0;                               // IST (Interrupt Stack Table)
    entry->zero = 0;                              // зарезервировано
}
