#include <cmd.h>
#include <stdio.h>
#include <string.h>
#include <arch/x86/screen.h>

static int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

void cmd_process(const char* command) {
    kputc('\n');

    if (strcmp(command, "help") == 0) {
        kputs("Available commands:\n");
        kputs("  help  - Show this help message\n");
        kputs("  clear - Clear the screen\n");
    } else if (strcmp(command, "clear") == 0) {
        x86_clear_screen();
    } else {
        kputs("Unknown command: ");
        kputs(command);
        kputs("\n");
    }
}
