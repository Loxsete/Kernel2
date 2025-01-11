#include <arch/x86/screen.h>
#include <hal/stdio.h>
#include <stdint.h>

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f

void set_cursor(int offset) {
    offset /= 2;
    outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    outb(VGA_DATA_REGISTER, (unsigned char) (offset >> 8));
    outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    outb(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}

int get_cursor() {
    outb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = inb(VGA_DATA_REGISTER) << 8;
    outb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += inb(VGA_DATA_REGISTER);
    return offset * 2;
}

void set_char_at_video_memory(char character, int offset) {
    unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
    vidmem[offset] = character;
    vidmem[offset + 1] = WHITE_ON_BLACK;
}

void memory_copy(char *source, char *dest, int nbytes) {
    for (int i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

int scroll_ln(int offset) {
    memory_copy(
            (char *) (get_offset(0, 1) + VIDEO_ADDRESS),
            (char *) (get_offset(0, 0) + VIDEO_ADDRESS),
            MAX_COLS * (MAX_ROWS - 1) * 2
    );

    for (int col = 0; col < MAX_COLS; col++) {
        set_char_at_video_memory(' ', get_offset(col, MAX_ROWS - 1));
    }

    return offset - 2 * MAX_COLS;
}

int get_offset(int col, int row) {
    return 2 * (row * MAX_COLS + col);
}

int get_row_from_offset(int offset) {
    return offset / (2 * MAX_COLS);
}

int move_offset_to_new_line(int offset) {
    return get_offset(0, get_row_from_offset(offset) + 1);
}


void x86_clear_screen(void) {
    for (int i = 0; i < MAX_COLS * MAX_ROWS; ++i) {
        set_char_at_video_memory(' ', i * 2);
    }
    set_cursor(get_offset(0, 0));
}

void x86_putc(const char ch) {
    int offset = get_cursor();

    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        offset = scroll_ln(offset);
    }

    if (ch == '\n') {
        offset = move_offset_to_new_line(offset);
    } else {
        set_char_at_video_memory(ch, offset);
        offset += 2;
    }

    set_cursor(offset);
}
