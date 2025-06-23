#include <stdint.h>
#include "io.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

static uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;
static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

void print_char(char c) {
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else {
        vga_buffer[cursor_row * VGA_WIDTH + cursor_col] = (0x0F << 8) | c;
        cursor_col++;
        if (cursor_col >= VGA_WIDTH) {
            cursor_col = 0;
            cursor_row++;
        }
    }
    if (cursor_row >= VGA_HEIGHT) {
        cursor_row = 0; // No scrolling for simplicity
    }
}

void print(const char* str) {
    while (*str) print_char(*str++);
}
