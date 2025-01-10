#include <hal/init.h> // arch_init
#include <stdio.h> // kputX

void kernel_main(void) {
    arch_init();
    kputs("ASINC Startinc \n");
    kputs("OS booted!\n");
    kputc('\n');
    // keyboard_handler_main();
}
