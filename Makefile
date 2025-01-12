ARCH := x86
KERNEL := build/arch/$(ARCH)/kernel.elf
GRUB_ISO := build/arch/$(ARCH)/grub.iso
KCFLAGS := $(CFLAGS) -ffreestanding -fno-stack-protector -I include -Wall -Wextra -Wpedantic -nostdinc

# Добавляем dyn_mem.c в список исходников
KSOURCES := $(shell find src/ -name '*.c' ! -path "*/arch/*") \
            $(shell find src/arch/$(ARCH) -name "*.c")


ARCH_ASMSOURCES := $(shell find src/arch/$(ARCH) -name '*.asm')

KOBJECTS := $(patsubst src/%.c, build/%.o, $(KSOURCES))
ARCH_ASMOBJECTS := $(patsubst src/%.asm, build/%.o, $(ARCH_ASMSOURCES))
ARCH_LINKER := src/arch/$(ARCH)/linker.ld

OBJECTS := $(ARCH_ASMOBJECTS) $(KOBJECTS)

all: $(KERNEL) $(GRUB_ISO)

build/%.o : src/%.c $(ALWAYS_REBUILD)
	mkdir -p $(dir $@) && \
	$(CC) -c $(KCFLAGS) $< -o $@

# TODO: rewrite asm sources to gnu assembler
build/%.o : src/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $< -o $@

$(KERNEL): $(OBJECTS) $(ARCH_LINKER)
	mkdir -p $(dir $@) && \
	$(LD) -o $@ -T $(ARCH_LINKER) $(OBJECTS)

$(GRUB_ISO): $(KERNEL)
	mkdir -p $(dir $@) && \
	grub-mkrescue -o $@ $(KERNEL) grub/

clean:
	rm -fr build
