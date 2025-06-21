#include "kbd.h"
#include "port.h"
#include "io.h"
#include "isr.h"

#define KBD_DATA_PORT 0x60
#define PIC1_COMMAND 0x20
#define PIC_EOI 0x20

static char scancode_to_ascii(uint8_t sc) {
    static const char map[128] = {
        0, 27, '1','2','3','4','5','6','7','8',
        '9','0','-','=', '\b', '\t',
        'q','w','e','r','t','y','u','i','o','p','[',']','\n',
        0, 'a','s','d','f','g','h','j','k','l',';','\'','`',
        0, '\\','z','x','c','v','b','n','m',',','.','/',
        0, '*', 0, ' '
    };
    return (sc > 127) ? 0 : map[sc];
}

void keyboard_handler_main() {
    uint8_t scancode = inb(KBD_DATA_PORT);
    if (scancode & 0x80) return; // Key release

    char c = scancode_to_ascii(scancode);
    if (c) print_char(c);

    // Send EOI
    outb(PIC1_COMMAND, PIC_EOI);
}

// ISR stub and registration in assembly needed here

void keyboard_install() {
    // Remap PIC, unmask IRQ1, register ISR for keyboard IRQ1
    // Implementation detail depends on your PIC and IDT setup
    // Enable IRQ1 (keyboard) on PIC
    uint8_t mask = inb(0x21);
    mask &= ~(1 << 1);      // Clear bit 1
    outb(0x21, mask);
}

