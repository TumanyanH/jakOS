#include <stdint.h>
#include "io.h"
#include "isr.h"
#include "kbd.h"
#include "idt.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void clear_screen() {
    volatile uint16_t* vga = (uint16_t*)VGA_ADDRESS;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga[i] = (0x0F << 8) | ' ';  // White text on black background
    }
}

void temp_open_irq0() {
    // Mask IRQ0 and all others except IRQ1
    uint8_t master_mask = inb(0x21);
    master_mask |= 0x01;   // Mask bit 0 (IRQ0)
    master_mask &= ~(1 << 1);  // Unmask IRQ1
    outb(0x21, master_mask);
    outb(0xA1, 0xFF);      // Mask all slave IRQs
}

void kernel_main() {
    clear_screen();
    print("Welcome to jakOS!\n> ");
    
    gdt_init();

    pic_remap();
    idt_init();
    // temp_open_irq0();
    keyboard_install();

    print_com1("Hello");
    __asm__ volatile ("sti");

    while (1) {
        __asm__ volatile ("hlt\n\t");
    }
}
