[BITS 32]
%macro isr_stub 1
global isr%1
isr%1:
    cli
    push dword 0     ; push dummy error code if none
    push dword %1    ; push interrupt number
    jmp isr_common_stub
%endmacro

isr_stub 0
isr_stub 1
isr_stub 2
isr_stub 3
isr_stub 4
isr_stub 5
isr_stub 6
isr_stub 7
isr_stub 8
isr_stub 9
isr_stub 10
isr_stub 11
isr_stub 12
isr_stub 13
isr_stub 14
isr_stub 15
isr_stub 16
isr_stub 17
isr_stub 18
isr_stub 19
isr_stub 20
isr_stub 21
isr_stub 22
isr_stub 23
isr_stub 24
isr_stub 25
isr_stub 26
isr_stub 27
isr_stub 28
isr_stub 29
isr_stub 30
isr_stub 31

extern isr_handler
isr_common_stub:
    pusha
    call isr_handler
    popa
    add esp, 8
    iretd