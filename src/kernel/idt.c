#include "idt.h"

struct idt_entry {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

#define IDT_ENTRIES 256
struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

extern void isr0(), isr1(), isr2(), isr3(), isr4(), isr5(), isr6(), isr7();
extern void isr8(), isr9(), isr10(), isr11(), isr12(), isr13(), isr14(), isr15();
extern void isr16(), isr17(), isr18(), isr19(), isr20(), isr21(), isr22(), isr23();
extern void isr24(), isr25(), isr26(), isr27(), isr28(), isr29(), isr30(), isr31();

void* isr_table[32] = {
    isr0,isr1,isr2,isr3,isr4,isr5,isr6,isr7,
    isr8,isr9,isr10,isr11,isr12,isr13,isr14,isr15,
    isr16,isr17,isr18,isr19,isr20,isr21,isr22,isr23,
    isr24,isr25,isr26,isr27,isr28,isr29,isr30,isr31
};

void pic_remap() {
    outb(0x20, 0x11); // start init
    outb(0xA0, 0x11);

    outb(0x21, 0x20); // master PIC vector offset = 0x20
    outb(0xA1, 0x28); // slave PIC vector offset = 0x28

    outb(0x21, 0x04); // tell master there is a slave at IRQ2
    outb(0xA1, 0x02); // tell slave its cascade identity

    outb(0x21, 0x01); // set 8086 mode
    outb(0xA1, 0x01);

    outb(0x21, 0x0); // unmask all (optional — or use your mask later)
    outb(0xA1, 0x0);
}

void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
}

extern void keyboard_handler();

void idt_init() {

    idtp.limit = sizeof(struct idt_entry) * IDT_ENTRIES - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < IDT_ENTRIES; i++)
        idt_set_gate(i, 0, 0, 0);

    // excep debugg    
    for (int i = 0; i < 32; i++) {
        idt_set_gate(i, (uint32_t)isr_table[i], 0x08, 0x8E);
    }

    // Set keyboard handler (IRQ1 = 0x21)
    idt_set_gate(0x21, (uint32_t)keyboard_handler, 0x08, 0x8E);

    // Load IDT
    __asm__ volatile ("lidt (%0)" : : "r" (&idtp));
}
