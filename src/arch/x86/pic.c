#include <arch/x86/pic.h>
#include <arch/x86/io.h>


#define PIC1_CMD     0x20
#define PIC1_DATA    0x21

#define PIC2_CMD     0xA0
#define PIC2_DATA    0xA1

#define PIC_CMD_EOI  0x20
#define PIC_READ_ISR 0x0b

void x86_pic_init(void) {
    // Enable pic
    outb(PIC1_DATA, 0x0);
    outb(PIC2_DATA, 0x0);
}

static x86_pic_isr_handler_function x86_pic_isr_handlers[16];

void x86_pic_isr_handler(void){
    uint8_t isr = x86_pic_get_isr(1);
    if(isr != 0xff)
        isr |= 8;
    else
        isr = x86_pic_get_isr(0);
    x86_pic_isr_handler_function handler = x86_pic_isr_handlers[isr];
    if(handler != 0)
        handler();
    x86_pic_send_eoi();
}

void x86_pic_set_isr_handler(uint8_t irq, x86_pic_isr_handler_function handler){
    if(irq<16)
        x86_pic_isr_handlers[irq] = handler;
}

uint8_t x86_pic_get_isr(uint8_t pic){
    uint8_t pic_io = (!pic) ? PIC1_CMD : PIC2_CMD;
    outb(pic_io, PIC_READ_ISR);
    uint8_t isr = inb(pic_io);
    if(isr == 0) return 0xff;

    uint8_t isr_ = 0;
    while(!(isr&1)){
        isr_++;
        isr>>=1;
    }
    return isr_;
}

void x86_pic_send_eoi(void){
    if(x86_pic_get_isr(1) != 0xff)
        outb(PIC2_CMD, PIC_CMD_EOI);
    outb(PIC1_CMD, PIC_CMD_EOI);
}
