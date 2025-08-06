#include <hal/init.h> // arch_init
#include <stdio.h> // kputX
#include <stdint.h>
#include <ipl_data.h> // cmdline
#include <arch/x86/screen.h> // Video driver

// test cmdline
static char ipl_info_level;
void process_cmdline(char* cmdline);
void show_ipl_info(uint8_t infolevel);
void demo_video_driver(void);

void kernel_main(void) {
    arch_init();
    
    // Демонстрация видео драйвера
    
    
    kputs("ASINC Startinc \n");
    kputs("OS booted!\n");
    kputc('\n');
    demo_video_driver();
    process_cmdline(ipl_cmdline);
    
    // test cmdline
    if (ipl_info_level)
        show_ipl_info(ipl_info_level);

    while(1);
}

void
process_cmdline(char* cmdline)
{
    // test cmdline
    ipl_info_level = cmdline && ('@' == cmdline[0]);

}

void 
show_ipl_info(uint8_t infolevel)
{
    if (infolevel > 0)
    {
        if (bootloader_name)
        kputs("\n" "IPLoader: "), kputs(bootloader_name);

        if (ipl_cmdline)
            kputs("\n" "IPL command line : "), kputs(ipl_cmdline);

        if (initrd)
            kputs("\n" "INITRD : "), kputs(initrd_cmd);

        if (ipl_module_count)
        {
            kputs("\n" "System modules and data sets:");
            for (uint32_t imodule = 0; imodule != ipl_module_count; ++imodule)
            {
                char* cmdline = ipl_modules[imodule].cmdline;
                if ( cmdline ) 
                    kputs("\n\t"), kputs(ipl_modules[imodule].cmdline);
            }
        }
        kputc('\n');
    }

}

void demo_video_driver(void)
{
    // Очистка экрана
    x86_clear_screen();
    
    // Демонстрация базовых функций
    x86_puts("=== demo video driver ===\n\n");
    
    // Демонстрация цветного текста
    x86_puts_color("red text on black background\n", VGA_RED, VGA_BLACK);
    x86_puts_color("green text on blue background\n", VGA_GREEN, VGA_BLUE);
    x86_puts_color("yellow text on red background\n", VGA_LIGHT_BROWN, VGA_RED);
    x86_puts_color("white text on green background\n", VGA_WHITE, VGA_GREEN);
    
    x86_puts("\n");
    
    // Демонстрация различных цветов
    x86_puts("patern colors:\n");
    
    for (int fg = 0; fg < 16; fg++) {
        x86_puts_color("█", fg, VGA_BLACK);
    }
    x86_puts("\n");
    
    // Демонстрация изменения цвета
    x86_puts("change color text: ");
    x86_set_foreground(VGA_LIGHT_BLUE);
    x86_puts("blue ");
    x86_set_foreground(VGA_LIGHT_GREEN);
    x86_puts("green ");
    x86_set_foreground(VGA_LIGHT_RED);
    x86_puts("red\n");
    
    // Возврат к стандартному цвету
    x86_set_color(VGA_LIGHT_GREY, VGA_BLACK);
    
    x86_puts("\ndemo finish!\n");
}
