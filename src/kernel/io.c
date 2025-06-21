#include <stdint.h>
#include "io.h"

volatile uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;
static uint16_t cursor_pos = 0;

void print_char(char c) {
    VGA_MEMORY[cursor_pos++] = (0x0F << 8) | c; // White on black
}

void print(const char* str) {
    while (*str) print_char(*str++);
}
