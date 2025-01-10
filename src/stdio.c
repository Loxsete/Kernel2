#include <stdio.h>
#include <hal/stdio.h>

void kputs(const char* str){
    while(*str)
        kputc(*str++);
}

void kputc(const char ch){
    arch_putc(ch);
}
