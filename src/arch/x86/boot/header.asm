section .multiboot_header
extern start
header_start:
	; magic number
	dd 0xe85250d6 ; multiboot2
	; architecture
	dd 0 ; protected mode i386
	; header length
	dd header_end - header_start
	; checksum
	dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

	; end tag
	dw 0
	dw 0
	dd 8
header_end:
	dd 0x1badb002, 1 << 16, - 0x1badb002 - (1<<16)
	dd header_end
	dd header_start
	dd 0
	dd 0 ;bss_end_addr
	dd start
