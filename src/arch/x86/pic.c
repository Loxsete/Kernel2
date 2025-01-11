#include <arch/x86/pic.h>
#include <arch/x86/io.h>

#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

void x86_pic_init(void) {
    // Disable pic
    outb(PIC1_DATA, 0xff);
    outb(PIC2_DATA, 0xff);
}
