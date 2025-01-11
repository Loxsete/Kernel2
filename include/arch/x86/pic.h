#pragma once

#include <stdint.h>

typedef void (*x86_pic_isr_handler_function)(void);

void x86_pic_init(void);
uint8_t x86_pic_get_isr(uint8_t pic);
void x86_pic_send_eoi(void);
void x86_pic_isr_handler(void);
void x86_pic_set_isr_handler(uint8_t irq, x86_pic_isr_handler_function handler);
