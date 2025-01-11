#include <arch/x86/screen.h>
#include <arch/x86/idt.h>
#include <arch/x86/pic.h>
#include <hal/init.h>

void arch_init(void){
    x86_clear_screen();
    x86_idt_init();
    x86_pic_init(); // TODO: apic
}
