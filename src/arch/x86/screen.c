#include <arch/x86/screen.h>
#include <hal/stdio.h>
#include <stdint.h>
#include <string.h>

static uint16_t* screen_buf = (uint16_t*)0xb8000;

#define NUM_COLS 80
#define NUM_ROWS 25

static uint16_t screen_pos = 0;

void x86_clear_screen(void) {
    for(size_t i = 0; i < NUM_COLS*NUM_ROWS; i++)
        screen_buf[i] = 0;
}

void x86_putc(const char ch) {
    if(screen_pos >= ((NUM_ROWS - (ch=='\n')) * NUM_COLS)){
        memcpy(screen_buf, screen_buf + NUM_COLS, (screen_pos - NUM_COLS)*2);
        screen_pos -= NUM_COLS - (screen_pos % NUM_COLS);
        for(int i = 0; i < NUM_COLS; i++)
            screen_buf[screen_pos + i] = 0;
    }
    if(ch == '\n'){
        screen_pos += NUM_COLS - (screen_pos % NUM_COLS);
        return;
    }
    screen_buf[screen_pos++] = (0x0f << 8) | ch;
}
