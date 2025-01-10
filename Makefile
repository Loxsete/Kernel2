# Находим все .c файлы в src/impl/kernel
kernel_c_source_files := $(shell find src/impl/kernel -name '*.c')
# Находим все .asm файлы в src/impl/x86_64/boot
x86_64_asm_source_files := $(shell find src/impl/x86_64/boot -name '*.asm')

# Создаем объектные файлы для каждого .c файла
kernel_object_files := $(patsubst src/impl/kernel/%.c, build/kernel/%.o, $(kernel_c_source_files))
# Создаем объектные файлы для каждого .asm файла
x86_64_asm_object_files := $(patsubst src/impl/x86_64/boot/%.asm, build/x86_64/boot/%.o, $(x86_64_asm_source_files))

# Все объектные файлы
all_object_files := $(kernel_object_files) $(x86_64_asm_object_files)

# Правило для компиляции .c файлов в объектные файлы
$(kernel_object_files): build/kernel/%.o : src/impl/kernel/%.c
	mkdir -p $(dir $@) && \
	gcc -c -I src/intf -ffreestanding $< -o $@

# Правило для компиляции .asm файлов в объектные файлы
$(x86_64_asm_object_files): build/x86_64/boot/%.o : src/impl/x86_64/boot/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $< -o $@

.PHONY: build-kernel
build-kernel: $(all_object_files)
	mkdir -p dist/x86_64 && \
	ld -n -o dist/x86_64/kernel.bin -T targets/x86_64/linker.ld $(all_object_files) && \
	cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso