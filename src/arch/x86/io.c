#include <arch/x86/io.h>

void outb(uint32_t port, uint8_t val) {
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

uint8_t inb(uint32_t port) {
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
    : "=a"(ret)
    : "Nd"(port)
    : "memory");
    return ret;
}
