#include <arch/x86/screen.h> // Предполагается, что эти функции определены здесь
#include <hal/stdio.h>      // Для kputs и kputc
#include <stdint.h>

#define MAX_INPUT_LENGTH 256

// Функция для сравнения строк
int my_strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

void shell_loop(void) {
    char input[MAX_INPUT_LENGTH];

    while (1) {
        x86_clear_screen(); // Очищаем экран
        kputs("simple_shell> "); // Подсказка для ввода

        // Чтение ввода
        int i = 0;
        char ch;
        while (1) {
            ch = x86_kbd_read(); // Чтение символа с клавиатуры
            if (ch == '\n') { // Если нажата клавиша Enter
                break;
            } else if (ch == 127) { // Если нажата клавиша Backspace
                if (i > 0) {
                    i--;
                    kputc('\b'); // Удаляем символ на экране
                    kputc(' ');  // Затираем символ
                    kputc('\b'); // Возвращаем курсор назад
                }
            } else if (i < MAX_INPUT_LENGTH - 1) { // Проверка на переполнение
                input[i++] = ch; // Сохраняем символ
                kputc(ch); // Выводим символ на экран
            }
        }
        input[i] = '\0'; // Завершаем строку

        // Обработка команды
        if (my_strcmp(input, "exit") == 0) {
            break; // Выход из оболочки
        } else {
            kputs("\nYou entered: ");
            kputs(input);
            kputs("\nPress Enter to continue...");
            while (x86_kbd_read() != '\n'); // Ожидание нажатия Enter
        }
    }
}

int main(void) {
    keyboard_init(); // Инициализация клавиатуры
    shell_loop(); // Запуск оболочки
    return 0;
}
