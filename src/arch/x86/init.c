#include <arch/x86/screen.h>
#include <hal/init.h>

void arch_init(void){
    x86_clear_screen();
}
