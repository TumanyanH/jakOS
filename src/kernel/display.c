#include "display.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void clear_screen() {
    volatile uint16_t* vga = (uint16_t*)VGA_ADDRESS;
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga[i] = (0x0F << 8) | ' ';
    }
}