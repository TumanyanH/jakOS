BITS 32
align 4

section .text
global multiboot_header
multiboot_header:
    dd 0x1BADB002         ; Magic
    dd 0x00010003         ; Flags (align + mem info)
    dd -(0x1BADB002 + 0x00010003) ; Checksum

section .note.GNU-stack noalloc
