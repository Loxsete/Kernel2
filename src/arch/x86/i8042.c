// src/arch/x86/i8042.c

#include <arch/x86/i8042.h>
#include <arch/x86/pic.h>
#include <arch/x86/io.h>
#include <keyboard_map.h>
#include <cmd.h>
#include <stdio.h>

#define KBD_DATA_PORT 0x60
#define INPUT_BUFFER_SIZE 256

static char input_buffer[INPUT_BUFFER_SIZE];

static size_t input_pos = 0;

char x86_kbd_read(void)
{
	while ((inb(0x64) & 0x01) == 0);
	return inb(KBD_DATA_PORT);
}

void kbd_handler(void)
{
    uint8_t scancode = x86_kbd_read();

    if (scancode & 0x80)
    {
        return;
    }

    if (scancode == 0x0E)
    {
        if (input_pos > 0)
        {
            input_pos--;
            input_buffer[input_pos] = 0;
            kputc('\b');
            kputc(' ');
            kputc('\b');
        }
        return;
    } else if (scancode == 0x1C)
    {
        if (input_pos > 0)
        {
            input_buffer[input_pos] = 0;
            cmd_process(input_buffer);
            input_pos = 0;
            input_buffer[0] = 0;
        }
        kputc('\n');
        kputc('>');
        kputc(' ');
        return;
    }

    char c = keyboard_map[scancode];  // Added semicolon
    if (c != 0 && input_pos < INPUT_BUFFER_SIZE - 1)
    {
        input_buffer[input_pos++] = c;
        input_buffer[input_pos] = 0;
        kputc(c);
    }
}

// Input-output

void x86_i8042_init(void)
{
	x86_pic_set_isr_handler(1, kbd_handler);
	kputs("> ");
}
