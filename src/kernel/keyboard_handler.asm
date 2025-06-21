[BITS 32]
extern keyboard_handler_main

global keyboard_handler
keyboard_handler:
    pusha
    call keyboard_handler_main
    popa
    iretd
