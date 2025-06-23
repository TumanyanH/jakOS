; filepath: src/kernel/keyboard_handler.asm
[BITS 32]
extern keyboard_handler_main
global keyboard_handler
keyboard_handler:
    pusha
    in al, 0x60
    push eax
    call keyboard_handler_main
    add esp, 4
    popa
    iretd