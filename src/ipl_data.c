#include <stdint.h>
#include <ipl_data.h>

 module_info_t* ipl_modules;
 uint32_t ipl_module_count;

 void* initrd;
 uint32_t initrd_size;
 char* initrd_cmd; // initrd's name or command

//  command line
 char* ipl_cmdline;

 char* bootloader_name;
