#include <stdint.h>
#include <stddef.h>
#include <stdarg.h> 
#include "print.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#define COM1_PORT 0x3F8

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

int serial_is_transmit_empty() {
    // Read the Line Status Register (LSR) at offset 5
    // Check if bit 5 (0x20) is set
    return inb(COM1_PORT + 5) & 0x20;
}

void print_char_COM1 (char c) {
    while (serial_is_transmit_empty() == 0);
    outb(COM1_PORT, c);
}

void print_com1 (char *str) {
    while (*str) print_char_COM1(*str++);
}

void print(const char* str) {
    while (*str) print_char(*str++);
}

static void print_int(int value) {
    char buffer[12];
    int i = 0;
    if (value == 0) {
        print_char('0');
        return;
    }
    if (value < 0) {
        print_char('-');
        value = -value;
    }
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }
    while (--i >= 0)
        print_char(buffer[i]);
}

static void print_hex(unsigned int value) {
    char hex[] = "0123456789ABCDEF";
    print("0x");
    for (int i = 7; i >= 0; i--) {
        print_char(hex[(value >> (i * 4)) & 0xF]);
    }
}

static void print_int_com1(int value) {
    char buffer[12];
    int i = 0;
    if (value == 0) {
        print_char_COM1('0');
        return;
    }
    if (value < 0) {
        print_char_COM1('-');
        value = -value;
    }
    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }
    while (--i >= 0)
        print_char_COM1(buffer[i]);
}

static void print_hex_com1(unsigned int value) {
    char hex[] = "0123456789ABCDEF";
    print_com1("0x");
    for (int i = 7; i >= 0; i--) {
        print_char_COM1(hex[(value >> (i * 4)) & 0xF]);
    }
}

void print_f(const char* format, ...) {
    va_list args;
    va_start(args, format);

    for (size_t i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && format[i + 1] != '\0') {
            i++;
            switch (format[i]) {
                case 's': {
                    const char* str = va_arg(args, const char*);
                    print(str);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);
                    print_char(c);
                    break;
                }
                case 'd':
                case 'i': {
                    int num = va_arg(args, int);
                    print_int(num);
                    break;
                }
                case 'x': {
                    unsigned int num = va_arg(args, unsigned int);
                    print_hex(num);
                    break;
                }
                case '%': {
                    print_char('%');
                    break;
                }
                default:
                    print_char('%');
                    print_char(format[i]);
                    break;
            }
        } else {
            print_char(format[i]);
        }
    }

    va_end(args);
}


void dbg_print_f(const char* format, ...) {
    va_list args;
    va_start(args, format);

    for (size_t i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && format[i + 1] != '\0') {
            i++;
            switch (format[i]) {
                case 's': {
                    const char* str = va_arg(args, const char*);
                    print_com1(str);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);
                    print_char_COM1(c);
                    break;
                }
                case 'd':
                case 'i': {
                    int num = va_arg(args, int);
                    print_int_com1(num);
                    break;
                }
                case 'x': {
                    unsigned int num = va_arg(args, unsigned int);
                    print_hex_com1(num);
                    break;
                }
                case '%': {
                    print_char_COM1('%');
                    break;
                }
                default:
                    print_char_COM1('%');
                    print_char_COM1(format[i]);
                    break;
            }
        } else {
            print_char_COM1(format[i]);
        }
    }

    va_end(args);
}
