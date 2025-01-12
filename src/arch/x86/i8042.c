#include <arch/x86/i8042.h>
#include <arch/x86/pic.h>
#include <stdio.h>
#include <arch/x86/io.h> // Заголовок для работы с I/O портами
#include <keyboard_map.h> // Подключаем заголовок с картой клавиатуры


#define KBD_DATA_PORT 0x60 // Порт данных клавиатуры

// Функция для чтения символа с клавиатуры
char x86_kbd_read(void) {
    // Ждем, пока данные не будут готовы к чтению
    while ((inb(0x64) & 0x01) == 0); // Проверяем флаг данных в порту 0x64
    return inb(KBD_DATA_PORT); // Читаем символ из порта данных
}

// Обработчик клавиатуры
void kbd_handler(void) {
    uint8_t ch = x86_kbd_read(); // Читаем символ с клавиатуры
    if (keyboard_map[ch] != 0) // Проверяем, что символ в пределах массива и не нулевой
        kputc(keyboard_map[ch]); // Выводим соответствующий символ на экран
}

// Функция для инициализации драйвера ввода-вывода
void x86_i8042_init(void) {
    x86_pic_set_isr_handler(1, kbd_handler); // Устанавливаем обработчик для клавиатуры
}
