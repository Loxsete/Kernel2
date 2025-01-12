#include <arch/x86/i8042.h>
#include <arch/x86/screen.h>
#include <arch/x86/idt.h>
#include <arch/x86/pic.h>
#include <hal/init.h>
#include <hal/driver.h>
#include <hal/stdio.h>
#include <stddef.h>
#include <stdio.h>
#include <arch/x86/io.h> // Заголовок для работы с I/O портами

// Функция для инициализации драйвера ввода-вывода
void* x86_io_driver_new(void) {
    x86_clear_screen(); // Очищаем экран
    x86_i8042_init();
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
    add_driver(&x86_io_driver_ops); // Добавляем драйвер
}
