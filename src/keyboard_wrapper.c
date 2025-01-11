
#include "keyboard_wrapper.h"
#include "keyboard_map.h" // Карта клавиатуры
#include <arch/x86/io.h>  // Для работы с I/O портами

#define KEYBOARD_BUFFER_SIZE 256

// Буфер клавиатуры
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static volatile size_t buffer_head = 0;
static volatile size_t buffer_tail = 0;

static keyboard_event_handler_t user_event_handler = NULL;

// Добавление символа в буфер
static void keyboard_buffer_push(char c) {
    size_t next = (buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
    if (next != buffer_tail) { // Проверяем, чтобы буфер не переполнился
        keyboard_buffer[buffer_head] = c;
        buffer_head = next;
    }
}

// Извлечение символа из буфера
static char keyboard_buffer_pop(void) {
    if (buffer_head == buffer_tail) {
        return 0; // Буфер пуст
    }
    char c = keyboard_buffer[buffer_tail];
    buffer_tail = (buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

// Обработчик клавиатуры
static void default_kbd_handler(char scancode) {
    if (scancode < KEYBOARD_MAP_SIZE) {
        char c = keyboard_map[scancode];
        if (c) {
            if (user_event_handler) {
                user_event_handler(c);
            } else {
                keyboard_buffer_push(c);
            }
        }
    }
}

void register_keyboard_event_handler(keyboard_event_handler_t handler) {
    user_event_handler = handler;
}

char keyboard_getchar(void) {
    return keyboard_buffer_pop();
}

bool keyboard_has_data(void) {
    return buffer_head != buffer_tail;
}

void keyboard_wrapper_init(void) {
    // Установка обработчика клавиатуры
    x86_pic_set_isr_handler(1, default_kbd_handler);
}
