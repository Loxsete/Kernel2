#pragma once
// Initial Program Load data

typedef struct {
	void* begin;
	char* cmdline; // module name or command
	uint32_t size;
}module_info_t;

extern module_info_t* ipl_modules;
extern uint32_t ipl_module_count;

extern void* initrd;
extern uint32_t initrd_size;
extern char* initrd_cmd; // initrd's name or command

//  command line
extern char* ipl_cmdline;

extern char* bootloader_name;
