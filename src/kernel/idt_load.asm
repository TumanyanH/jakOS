BITS 32

global idt_load

idt_load:
    lidt [eax]
    ret
