BITS 32
align 4

global multiboot_header
multiboot_header:
    dd 0x1BADB002
    dd 0x00010003
    dd -(0x1BADB002 + 0x00010003)

section .note.GNU-stack "a"
