#include "isr.h"
#include "idt.h"
#include "io.h"

extern void keyboard_handler();

void isr_install() {
    idt_init();  // Ensure this is declared in a header (idt.h)

    idt_set_gate(33, (uint32_t)keyboard_handler, 0x08, 0x8E);  // IRQ1 = 0x20 + 1
}
