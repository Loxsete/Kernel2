#include <hal/init.h> // arch_init
#include <stdio.h> // kputX
#include <stdint.h>
#include <ipl_data.h> // cmdline

// test cmdline
static char ipl_info_level;
void process_cmdline(char* cmdline);
void show_ipl_info(uint8_t infolevel);

void kernel_main(void) {
    arch_init();
    kputs("ASINC Startinc \n");
    kputs("OS booteds!\n");
    kputc('\n');
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