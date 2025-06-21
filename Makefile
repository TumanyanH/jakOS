CC=i686-elf-gcc
LD=i686-elf-ld
CFLAGS=-ffreestanding -O2 -Wall -Wextra
LDFLAGS=-T src/linker.ld -nostdlib

OUT_DIR=build
KERNEL=$(OUT_DIR)/kernel
ISO=$(OUT_DIR)/myos.iso

all: $(ISO)

$(KERNEL): src/kernel.c
	mkdir -p $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $(KERNEL).o
	$(LD) $(LDFLAGS) $(KERNEL).o -o $(KERNEL)

$(ISO): $(KERNEL)
	mkdir -p $(OUT_DIR)/iso/boot/grub
	cp $(KERNEL) $(OUT_DIR)/iso/boot/kernel
	cp boot/grub/grub.cfg $(OUT_DIR)/iso/boot/grub
	grub-mkrescue -o $(ISO) $(OUT_DIR)/iso

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(OUT_DIR)

install-deps:
	sudo apt update && \
	sudo apt install -y build-essential grub-pc-bin xorriso qemu-system-i386 \
	                    gcc-i686-linux-gnu binutils-i686-linux-gnu