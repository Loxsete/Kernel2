// initial program load
#include <arch/x86/ipl.h>
#include <ipl_data.h>
#include <mem.h>

#define MB2 0x36d76289
#define MB0	0x2BADB002
void mb2_ipl(void);
void mb_ipl(void);

void ipl()
{
	switch (boot_sig)
	{
	// https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#Machine-state
	case MB2:	mb2_ipl(); break;
	case MB0:	mb_ipl(); break;
    default:
    ipl_modules = 0;
    initrd_size = ipl_module_count = 0;
    ipl_cmdline = initrd_cmd = bootloader_name = 0;
	}
}
uint32_t boot_sig;
uint32_t boot_context;

//https://www.gnu.org/software/grub/manual/multiboot2/multiboot.html#Boot-information-format

//multiboot2.h
#define MULTIBOOT_TAG_ALIGN                  8
#define MULTIBOOT_TAG_TYPE_END               0
#define MULTIBOOT_TAG_TYPE_CMDLINE           1
#define MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME  2
#define MULTIBOOT_TAG_TYPE_MODULE            3

// TODO: add tagtypes when we need'em

#define multiboot_uint32_t uint32_t

struct multiboot_tag
{
    multiboot_uint32_t type;
    multiboot_uint32_t size;
};

struct multiboot_tag_string
{
    multiboot_uint32_t type;
    multiboot_uint32_t size;
    char string[];  // C99 FAM
};

struct multiboot_tag_module
{
    multiboot_uint32_t type;
    multiboot_uint32_t size;
    multiboot_uint32_t mod_start;
    multiboot_uint32_t mod_end;
    char cmdline[]; // C99 FAM
};

void mb2_ipl(void)
{
    // process multiboot2 "boot info"
    char* addr = (char*)(uintptr_t)boot_context;

    ipl_modules = 0;
    initrd_size = ipl_module_count = 0;
    ipl_cmdline = initrd_cmd = bootloader_name = 0;

    int modules_count = 0;

    struct multiboot_tag* tag;
    for (tag = (struct multiboot_tag*)(addr + 8);
        tag->type != MULTIBOOT_TAG_TYPE_END;
        tag = (struct multiboot_tag*)((char*)tag
            + ((tag->size + 7) & ~7)))
    {
        switch (tag->type)
        {
        case MULTIBOOT_TAG_TYPE_CMDLINE:
            ipl_cmdline = ((struct multiboot_tag_string*)tag)->string;
            break;
        case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
            bootloader_name = ((struct multiboot_tag_string*)tag)->string;
            break;
        case MULTIBOOT_TAG_TYPE_MODULE:
            ++modules_count;
            break;
        }
    }

    //insane we are
    if (modules_count)
    {
        ipl_module_count = modules_count;
        ipl_modules = kalloc(modules_count * sizeof (module_info_t));

        int imod = 0;
        for (tag = (struct multiboot_tag*)(addr + 8);
            tag->type != MULTIBOOT_TAG_TYPE_END;
            tag = (struct multiboot_tag*)((char*)tag
                + ((tag->size + 7) & ~7)), ++imod)
            switch (tag->type)
            case MULTIBOOT_TAG_TYPE_MODULE:
                ipl_modules[imod].begin = (void*)(uintptr_t)((struct multiboot_tag_module*)tag)->mod_start,
                ipl_modules[imod].size = ((struct multiboot_tag_module*)tag)->mod_end
                      - ((struct multiboot_tag_module*)tag)->mod_start,
                ipl_modules[imod].cmdline = ((struct multiboot_tag_module*)tag)->cmdline;
    }
    if (ipl_module_count)
    {
        // Let the first module be the initrd ( QEMU style multiboot)
        initrd = ipl_modules[0].begin;
        initrd_cmd = ipl_modules[0].cmdline;
        initrd_size = ipl_modules[0].size;
    }
}

	// multiboot.h
#define MULTIBOOT_INFO_CMD_LINE 4
#define MULTIBOOT_INFO_MODS	8
#define MULTIBOOT_LOADER_NAME	0x200

typedef
struct multiboot_info
{
	uint32_t flags;
	uint32_t mem_lower, mem_upper;
	uint32_t boot_device;
	uint32_t cmdline;
	uint32_t mods_count;
	uint32_t mods_addr;
} multiboot_info_t;
void
mb_ipl(){

    ipl_modules = 0;
    initrd_size = ipl_module_count = 0;
    ipl_cmdline = initrd_cmd = bootloader_name = 0;

	multiboot_info_t *mbi = (multiboot_info_t*)(uint64_t)boot_context;
	if ( 0 == boot_context )
		return;
	
	if ( mbi->flags & MULTIBOOT_INFO_CMD_LINE )
		ipl_cmdline = (char*)(uint64_t)mbi->cmdline;

	if ( MULTIBOOT_INFO_MODS & mbi->flags )
		ipl_module_count = mbi->mods_count;
	if (ipl_module_count)
	    {
		ipl_modules = kalloc(ipl_module_count * sizeof (module_info_t));
		
		struct multiboot_mod_list {
			uint32_t mod_start, mod_end;
			uint32_t cmdline;
			uint32_t zero;
		} *modlist = (struct multiboot_mod_list*)(uint64_t)mbi->mods_addr;
			
		for ( uint32_t i = 0; i < ipl_module_count; ++i )
		{		
			ipl_modules[i].begin =(void*) modlist[i].mod_start;
			ipl_modules[i].size = modlist[i].mod_end-modlist[i].mod_start;
			ipl_modules[i].cmdline = (char*) modlist[i].cmdline;
		}
		// Let the first module be the initrd ( QEMU style multiboot)
		initrd = ipl_modules[0].begin;
		initrd_cmd = ipl_modules[0].cmdline;
		initrd_size = ipl_modules[0].size;
	    }
}

