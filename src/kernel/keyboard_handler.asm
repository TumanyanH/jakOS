[BITS 32]
extern keyboard_handler_main
global keyboard_handler
keyboard_handler:
    pusha
    in al, 0x60
    movzx eax, al
    push eax
    call keyboard_handler_main
    add esp, 4
    popa
    iretd