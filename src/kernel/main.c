#include <stdint.h>
#include "io.h"
#include "isr.h"
#include "kbd.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void clear_screen() {
    volatile uint16_t* vga = (uint16_t*)VGA_ADDRESS;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga[i] = (0x0F << 8) | ' ';  // White text on black background
    }
}

void kernel_main() {
    clear_screen();
    print("Welcome to jakOS!\n> ");
    isr_install();
    keyboard_install();

    // Enable interrupts here (sti)

    __asm__ volatile ("sti");

    // while(1) {
    //    __asm__ volatile ("hlt");
    // }
}
