#include "kernel.h"

void __stack_chk_fail(void) {
    // Можно вывести сообщение об ошибке или просто выйти
    while(1);
}


void kernel_main(void) {
    clear_screen();
    print("ASINC Startinc \n");
    print("OS booted!\n");
    kprint_newline();
    keyboard_handler_main();
    
   
    
}



