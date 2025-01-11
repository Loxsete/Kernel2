#include <arch/x86/screen.h>
#include <arch/x86/idt.h>
#include <arch/x86/pic.h>
#include <hal/init.h>
#include <hal/driver.h>
#include <hal/stdio.h>
#include <stddef.h>
#include <stdio.h>

void pit_handler(void) {
    kputc('.');
}

void kbd_handler(void) {
    kputc('?');
}

void* x86_io_driver_new(void){
    x86_clear_screen();
    return NULL;
}

void x86_io_driver_putc(void* _, const char ch){
    (void)_;
    x86_putc(ch);
}

driver_ops_t x86_io_driver_ops = {
    .new = x86_io_driver_new,
    .type = DRIVER_TYPE_IO,
    .type_ops = &(io_driver_ops_t){
        .putc = x86_io_driver_putc,
        .read = NULL,
    }
};

void arch_init(void){
    add_driver(&x86_io_driver_ops);
    x86_pic_init();
    x86_idt_init();
    x86_pic_set_isr_handler(0, pit_handler);
    x86_pic_set_isr_handler(1, kbd_handler);
}
