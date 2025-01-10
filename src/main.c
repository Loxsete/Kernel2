#include <kernel.h>

void kernel_main(void) {
    clear_screen();
    print("ASINC Startinc \n");
    print("OS booted!\n");
    kprint_newline();
    keyboard_handler_main();    
}
