#ifndef KEYBOARD_WRAPPER_H
#define KEYBOARD_WRAPPER_H

#include <stdbool.h>

// Объявление пользовательского обработчика
typedef void (*keyboard_event_handler_t)(char);

// Инициализация оболочки клавиатуры
void keyboard_wrapper_init(void);

// Регистрация пользовательского обработчика клавиш
void register_keyboard_event_handler(keyboard_event_handler_t handler);

// Получение символа из буфера
char keyboard_getchar(void);

// Проверка, есть ли данные в буфере
bool keyboard_has_data(void);

#endif // KEYBOARD_WRAPPER_H
