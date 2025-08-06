#include <arch/x86/screen.h>
#include <hal/stdio.h>
#include <stdint.h>
#include <string.h>

static uint16_t* screen_buf = (uint16_t*)0xb8000;



static uint16_t screen_pos = 0;
static uint8_t current_color = 0x0f; // Белый текст на черном фоне по умолчанию



void x86_clear_screen(void) {
    for(size_t i = 0; i < NUM_COLS*NUM_ROWS; i++)
        screen_buf[i] = 0;
}

void x86_set_color(uint8_t foreground, uint8_t background) {
    current_color = (background << 4) | (foreground & 0x0F);
}

void x86_set_foreground(uint8_t color) {
    current_color = (current_color & 0xF0) | (color & 0x0F);
}

void x86_set_background(uint8_t color) {
    current_color = (current_color & 0x0F) | ((color & 0x0F) << 4);
}

void x86_putc(const char ch) {
    if (ch == '\n') {
        screen_pos += NUM_COLS - (screen_pos % NUM_COLS);
        return;
    }

    if (ch == '\b') {
        if (screen_pos > 0) {
            screen_pos--;
            screen_buf[screen_pos] = (current_color << 8) | ' ';
        }
        return;
    }

    if (screen_pos >= ((NUM_ROWS - 1) * NUM_COLS)) {
        memcpy(screen_buf, screen_buf + NUM_COLS, (NUM_ROWS - 1) * NUM_COLS * 2);
        for (int i = 0; i < NUM_COLS; i++)
            screen_buf[(NUM_ROWS - 1) * NUM_COLS + i] = (current_color << 8) | ' ';
        screen_pos -= NUM_COLS;
    }

    screen_buf[screen_pos++] = (current_color << 8) | ch;
}

// Функции для вывода цветного текста
void x86_puts_color(const char* str, uint8_t foreground, uint8_t background) {
    uint8_t old_color = current_color;
    x86_set_color(foreground, background);
    while(*str) {
        if (*str == '\n') {
            x86_putc(*str++);
        } else {
            x86_putc(*str++);
        }
    }
    current_color = old_color;
}

void x86_putc_color(const char ch, uint8_t foreground, uint8_t background) {
    uint8_t old_color = current_color;
    x86_set_color(foreground, background);
    x86_putc(ch);
    current_color = old_color;
}
