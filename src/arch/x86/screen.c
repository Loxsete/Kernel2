#include <arch/x86/screen.h>
#include <hal/stdio.h>
#include <stdint.h>

static uint16_t* screen_buf = (uint16_t*)0xb8000;

#define NUM_COLS 80
#define NUM_ROWS 25

static uint16_t screen_pos = 0;

void x86_clear_screen(void) {
    for(size_t i = 0; i < NUM_COLS*NUM_ROWS; i++)
        screen_buf[i] = 0;
}

void x86_putc(const char ch) {
    if(screen_pos > (NUM_COLS * NUM_ROWS + (ch=='\n')))
        screen_pos -= NUM_COLS; // TODO: scroll up
    if(ch == '\n'){
        screen_pos += NUM_COLS - (screen_pos % NUM_COLS) ;
        return;
    }
    screen_buf[screen_pos++] = (0x0f << 8) | ch;
}
