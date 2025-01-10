#include "keyboard_map.h"
#include "types.h"
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdarg.h>

extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);
// Пример объявления функции clear_screen
void clear_screen(void);

unsigned int current_loc = 0;
char *vidptr = (char*)0xb8000;
unsigned int lines = 0; // Переменная для отслеживания количества строк
unsigned char current_color = 0x07; // Устанавливаем начальный цвет текста

struct IDT_entry {
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];

int atoi(const char *str) {
    int result = 0;
    int sign = 1;

    while (*str == ' ') str++;

    if (*str == '-' || *str == '+') {
        if (*str == '-') sign = -1;
        str++;
    }

    while (*str >= '0' && *str <= '9') {
        int digit = *str - '0';
        if (result > (INT_MAX - digit) / 10) {
            return (sign == 1) ? INT_MAX : INT_MIN;
        }
        result = result * 10 + digit;
        str++;
    }

    return sign * result;
}

void print(const char *str) {
    unsigned int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            current_loc += 80 - (current_loc % 80);
            lines++; // Увеличиваем количество строк
        } else {
            vidptr[current_loc * 2] = str[i];
            vidptr[current_loc * 2 + 1] = 0x07;
            current_loc++;
        }

        if (current_loc >= 80 * 25) {
            current_loc = 0;
        }
        i++;
    }

    // Проверяем количество строк
    if (lines >= LINES) {
        clear_screen(); // Очищаем экран, если строк больше 0
        lines = 0; // Сбрасываем счетчик строк
    }
}

void printn(int num) {
    char buffer[32]; 
    int i = 0, isNegative = 0;

    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    do {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    if (isNegative) {
        buffer[i++] = '-';
    }

    // Обратный порядок
    for (int j = i - 1; j >= 0; j--) {
        vidptr[current_loc * 2] = buffer[j];
        vidptr[current_loc * 2 + 1] = 0x07;
        current_loc++;

        if (current_loc >= 80 * 25) {
            current_loc = 0;
        }
    }
    
    // Проверяем количество строк
    if (lines >= LINES) {
        clear_screen(); // Очищаем экран, если строк больше 0
        lines = 0; // Сбрасываем счетчик строк
    }
}

void kprint_newline(void) {
    current_loc += (80 - (current_loc % 80));
    lines++; // Увеличиваем количество строк
}

void clear_screen(void) {
    unsigned int i = 0;
    while (i < SCREENSIZE) {
        vidptr[i++] = ' ';
        vidptr[i++] = 0x07;
    }
    current_loc = 0;
    lines = 0; // Сбрасываем количество строк
}

void keyboard_handler_main(void) {
    unsigned char status;
    char keycode;

    write_port(0x20, 0x20);
    status = read_port(KEYBOARD_STATUS_PORT);

    if (status & 0x01) {
        keycode = read_port(KEYBOARD_DATA_PORT);
        if (keycode < 0) return;
        if (keycode == ENTER_KEY_CODE) {
            kprint_newline();
            return;
        }
        if (keycode == BACKSPACE_KEY_CODE) {
            if (current_loc > 0) {
                current_loc--;
                vidptr[current_loc * 2] = ' ';
                vidptr[current_loc * 2 + 1] = 0x07;
            }
            return;
        }
        if (keycode < 128) {
            char c = keyboard_map[keycode];
            if (c != 0) {
                vidptr[current_loc * 2] = c;
                vidptr[current_loc * 2 + 1] = 0x07;
                current_loc++;
            }
        }
    }
}

void input(char *buffer, int max_size) {
    unsigned int index = 0;
    while (index < max_size - 1) {
        unsigned char status = read_port(KEYBOARD_STATUS_PORT);
        if (status & 0x01) {
            unsigned char keycode = read_port(KEYBOARD_DATA_PORT);
            if (keycode == ENTER_KEY_CODE) {
                break;
            }
            if (keycode < 128) {
                char c = keyboard_map[keycode];
                if (c != 0) {
                    if (c == '\b') {
                        if (index > 0) {
                            index--;
                            vidptr[current_loc * 2 - 2] = ' ';
                            vidptr[current_loc * 2 - 1] = 0x07;
                            current_loc--;
                        }
                    } else {
                        buffer[index++] = c;
                        vidptr[current_loc * 2] = c;
                        vidptr[current_loc * 2 + 1] = 0x07;
                        current_loc++;
                    }
                }
            }
        }
    }
    buffer[index] = '\0';
    kprint_newline();
}

int inputn() {
    char buffer[MAX_INPUT_SIZE];
    input(buffer, MAX_INPUT_SIZE);
    return atoi(buffer);
}

void idt_init(void) {
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    keyboard_address = (unsigned long)keyboard_handler;
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
    IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = INTERRUPT_GATE;
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

    write_port(0x20, 0x11);
    write_port(0xA0, 0x11);
    write_port(0x21, 0x20);
    write_port(0xA1, 0x28);
    write_port(0x21, 0x00);
    write_port(0xA1, 0x00);
    write_port(0x21, 0x01);
    write_port(0xA1, 0x01);

    write_port(0x21, 0xff);
    write_port(0xA1, 0xff);

    idt_address = (unsigned long)IDT;
    idt_ptr[0] = (sizeof(struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16;
    load_idt(idt_ptr);
}

void kb_init(void) {
    write_port(0x21, 0xFD);
}

unsigned long factorial(int n) {
    if (n < 0) return 0; // Факториал для отрицательных чисел не определен
    unsigned long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

void factorial_calculator(void) {
    print("\nFACTORIAL CALCULATOR");
    kprint_newline();
    int num;
    print("Please enter a number:");
    num = inputn();
    unsigned long result = factorial(num);
    printn(num);
    print("! = ");
    printn(result);
    print("\n");
    kprint_newline();
}

void decimal_to_binary(int n) {
    if (n == 0) {
        print("0");
        return;
    }
    char binary[32];
    int index = 0;
    while (n > 0) {
        binary[index++] = (n % 2) + '0';
        n /= 2;
    }
    for (int i = index - 1; i >= 0; i--) {
        vidptr[current_loc * 2] = binary[i];
        vidptr[current_loc * 2 + 1] = 0x07;
        current_loc++;
    }
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

// Рандом
unsigned int rand() {
    static unsigned int seed = 12345; // Начальное значение
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return seed % 3; // Получаем число от 0 до 2
}

// Задержка
void delay(unsigned int count) {
    for (volatile unsigned int i = 0; i < count; i++);
}

// Задать цвет текста в консоли
void color(unsigned char text_color) {
    if (text_color > 15) {
        return; 
    }

    current_color = text_color; // Устанавливаем цвет текста
}

void bgcolor(unsigned char bg_color) {
    // Проверяем, что цвет фона находится в допустимых пределах (0-15)
    if (bg_color > 15) {
        return; 
    }

    // Обновляем цвет фона для всех символов на экране
    for (unsigned int i = 0; i < SCREENSIZE; i += 2) {
        // Устанавливаем фон в старший ниббл
        vidptr[i + 1] = (vidptr[i + 1] & 0x0F) | (bg_color << 4); 
    }
}

