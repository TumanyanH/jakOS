#include "kbd.h"
#include "port.h"
#include "io.h"

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

void keyboard_handler_main(uint8_t scancode) {
    print_com1("kbd handler");
    if (scancode & 0x80) return; // Ignore key releases
    // char c = scancode_to_ascii(scancode);
    // if (c) print_char(c);
    outb(PIC1_COMMAND, PIC_EOI);
}

void keyboard_install() {
    // Only unmask IRQ1 (keyboard)
    uint8_t mask = 0xFF; // Mask all
    mask &= ~(1 << 1);   // Unmask IRQ1
    outb(0x21, mask); // <-------------------------- koza
}

