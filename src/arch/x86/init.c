#include <arch/x86/screen.h>
#include <arch/x86/idt.h>
#include <arch/x86/pic.h>
#include <hal/init.h>
#include <hal/driver.h>
#include <hal/stdio.h>
#include <stddef.h>
#include <stdio.h>
#include <arch/x86/io.h> // Заголовок для работы с I/O портами

#define KBD_DATA_PORT 0x60 // Порт данных клавиатуры

// Определяем коды специальных клавиш
#define KEY_SPACE 0x20
#define KEY_BACKSPACE 0x0E
#define KEY_UP 0x48
#define KEY_DOWN 0x50
#define KEY_LEFT 0x4B
#define KEY_RIGHT 0x4D

// Функция для чтения символа с клавиатуры
char x86_kbd_read(void) {
    // Ждем, пока данные не будут готовы к чтению
    while ((inb(0x64) & 0x01) == 0); // Проверяем флаг данных в порту 0x64
    return inb(KBD_DATA_PORT); // Читаем символ из порта данных
}

// Функция для стирания символа на экране
void x86_clear_last_char(void) {
    // Стираем последний символ на экране
    x86_putc('\b'); // Отправляем символ возврата каретки
    x86_putc(' ');  // Затираем символ пробелом
    x86_putc('\b'); // Возвращаем курсор назад
}

// Обработчик для PIT (Programmable Interval Timer)
void pit_handler(void) {
    // Здесь можно добавить код для обработки таймера, если нужно
}

// Обработчик клавиатуры
void kbd_handler(void) {
    char ch = x86_kbd_read(); // Читаем символ с клавиатуры

    // Обработка специальных клавиш
    switch (ch) {
        case KEY_SPACE:
            x86_putc(' '); // Выводим пробел
            break;
        case KEY_BACKSPACE:
            x86_clear_last_char(); // Стираем последний символ
            break;
        case KEY_UP:
            // Обработка стрелки вверх (можно добавить функциональность)
            break;
        case KEY_DOWN:
            // Обработка стрелки вниз (можно добавить функциональность)
            break;
        case KEY_LEFT:
            // Обработка стрелки влево (можно добавить функциональность)
            break;
        case KEY_RIGHT:
            // Обработка стрелки вправо (можно добавить функциональность)
            break;
        default:
            if (ch != 0) { // Проверяем, что символ не нулевой
                x86_putc(ch); // Выводим символ на экран
            }
            break;
    }
}

// Функция для инициализации драйвера ввода-вывода
void* x86_io_driver_new(void) {
    x86_clear_screen(); // Очищаем экран
    x86_pic_set_isr_handler(1, kbd_handler); // Устанавливаем обработчик для клавиатуры
    return NULL;
}

// Функция для вывода символа
void x86_io_driver_putc(void* _, const char ch) {
    (void)_;
    x86_putc(ch); // Выводим символ
}

// Описание операций драйвера
driver_ops_t x86_io_driver_ops = {
    .new = x86_io_driver_new,
    .type = DRIVER_TYPE_IO,
    .type_ops = &(io_driver_ops_t){
        .putc = x86_io_driver_putc,
        .read = NULL,
    }
};

// Функция инициализации архитектуры
void arch_init(void) {
    x86_pic_init(); // Инициализация PIC
    x86_idt_init(); // Инициализация IDT
    x86_pic_set_isr_handler(0, pit_handler); // Устанавливаем обработчик для PIT
    add_driver(&x86_io_driver_ops); // Добавляем драйвер
}