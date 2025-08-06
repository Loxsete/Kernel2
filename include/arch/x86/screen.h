#pragma once

#include <stdint.h>

// Цветовые константы VGA
#define VGA_BLACK 0x0
#define VGA_BLUE 0x1
#define VGA_GREEN 0x2
#define VGA_CYAN 0x3
#define VGA_RED 0x4
#define VGA_MAGENTA 0x5
#define VGA_BROWN 0x6
#define VGA_LIGHT_GREY 0x7
#define VGA_DARK_GREY 0x8
#define VGA_LIGHT_BLUE 0x9
#define VGA_LIGHT_GREEN 0xA
#define VGA_LIGHT_CYAN 0xB
#define VGA_LIGHT_RED 0xC
#define VGA_LIGHT_MAGENTA 0xD
#define VGA_LIGHT_BROWN 0xE
#define VGA_WHITE 0xF

// Размеры экрана
#define NUM_COLS 80
#define NUM_ROWS 25

// Базовые функции
void x86_clear_screen(void);
void x86_putc(const char ch);

// Функции управления цветом
void x86_set_color(uint8_t foreground, uint8_t background);
void x86_set_foreground(uint8_t color);
void x86_set_background(uint8_t color);

// Функции цветного вывода
void x86_putc_color(const char ch, uint8_t foreground, uint8_t background);
void x86_puts_color(const char* str, uint8_t foreground, uint8_t background);

// Вспомогательные функции для строк
static inline void x86_puts(const char* str) {
    while(*str) {
        x86_putc(*str++);
    }
}
