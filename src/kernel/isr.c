#include "isr.h"
#include "idt.h"
#include "io.h"

extern void keyboard_handler();

void isr_install() {
    idt_init();  // Ensure this is declared in a header (idt.h)
    idt_set_gate(33, (uint32_t)keyboard_handler, 0x08, 0x8E);  // IRQ1 = 0x20 + 1
}

void isr_handler(struct regs* r) {
    if (r->int_no == 14) {
        dbg_print_f("[KERNEL][VMM][ERR]: Page fault at address %x\n", r->err_code);
    }
}

extern void irq_handler();

void irq_handler() {
    print_com1("IRQ fired\n");
    outb(0x20, 0x20);
}
