TARGET = kernel-001
BUILD_DIR = build
ISO_DIR = isodir
ISO = jakos.iso

SRC_C = $(shell find src -name '*.c')
SRC_ASM = src/multiboot_header.asm
OBJ_C = $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(SRC_C))
OBJ_ASM = $(BUILD_DIR)/multiboot_header.o

CC = i686-elf-gcc
LD = i686-elf-ld
NASM = nasm
CFLAGS = -ffreestanding -O2 -Wall -Wextra -Iinclude -Isrc/kernel
LDFLAGS = -T linker.ld

.PHONY: all clean iso run check_deps

all: $(BUILD_DIR)/$(TARGET)

check_deps:
	sudo apt update && sudo apt install -y build-essential nasm grub-pc-bin xorriso qemu-system-i386

# Compile C source files
$(BUILD_DIR)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble idt_load.asm
$(BUILD_DIR)/idt_load.o: src/kernel/idt_load.asm
	mkdir -p $(dir $@)
	nasm -f elf32 $< -o $@

$(BUILD_DIR)/keyboard_handler.o: src/kernel/keyboard_handler.asm
	nasm -f elf32 $< -o $@

# Assemble the multiboot header
$(OBJ_ASM): $(SRC_ASM)
	mkdir -p $(dir $@)
	$(NASM) -f elf32 $< -o $@

# Link the kernel binary
$(BUILD_DIR)/$(TARGET): $(OBJ_ASM) $(OBJ_C) $(BUILD_DIR)/idt_load.o
	$(LD) $(LDFLAGS) -o $@ $^

# Build ISO image
iso: all
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(BUILD_DIR)/$(TARGET) $(ISO_DIR)/boot/$(TARGET)
	cp boot/grub/grub.cfg $(ISO_DIR)/boot/grub
	grub-mkrescue -o $(ISO) $(ISO_DIR)

# Run with QEMU
run: iso
	qemu-system-i386 -cdrom $(ISO)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) $(ISO)

