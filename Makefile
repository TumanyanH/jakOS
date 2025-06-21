CC=i686-linux-gnu-gcc
LD=i686-linux-gnu-ld
NASM=nasm
CFLAGS=-m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-m elf_i386 -T src/linker.ld -nostdlib
OUT_DIR=build
KERNEL=$(OUT_DIR)/kernel
ISO=$(OUT_DIR)/jakos.iso

.PHONY: all clean run iso

all: $(ISO)

$(ISO): $(KERNEL)
	mkdir -p $(OUT_DIR)/iso/boot/grub
	cp $(KERNEL) $(OUT_DIR)/iso/boot/kernel
	cp boot/grub/grub.cfg $(OUT_DIR)/iso/boot/grub
	grub-mkrescue -o $(ISO) $(OUT_DIR)/iso

$(KERNEL): src/kernel.c src/multiboot_header.asm src/linker.ld
	mkdir -p $(OUT_DIR)
	$(NASM) -f elf32 src/multiboot_header.asm -o $(OUT_DIR)/multiboot_header.o
	$(CC) $(CFLAGS) -c src/kernel.c -o $(OUT_DIR)/kernel.o
	$(LD) $(LDFLAGS) $(OUT_DIR)/multiboot_header.o $(OUT_DIR)/kernel.o -o $(KERNEL)

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(OUT_DIR)
	rm -rf iso
