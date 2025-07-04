[BITS 32]
global gdt_flush

gdt_flush:
    mov eax, [esp + 4] ; Get address of gdt_ptr
    lgdt [eax]

    ; Load new segment registers
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:flush_label
flush_label:
    ret