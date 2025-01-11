#include <arch/x86/screen.h>
#include <arch/x86/idt.h>
#include <arch/x86/pic.h>
#include <hal/init.h>
#include <stdio.h>

void pit_handler(void) {
    kputc('.');
}

void arch_init(void){
    x86_clear_screen();
    x86_pic_init();
    x86_idt_init();
    x86_pic_set_isr_handler(1, pit_handler);
}
