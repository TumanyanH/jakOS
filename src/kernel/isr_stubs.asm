[BITS 32]

extern isr_handler

%macro ISR_NOERR 1
global isr%1
isr%1:
    cli
    push dword 0          ; Push dummy error code
    push dword %1         ; Push interrupt number
    jmp isr_common_stub
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
    cli
    push dword %1         ; Push interrupt number
    jmp isr_common_stub
%endmacro

; ISRs with NO error code
ISR_NOERR 0   ; Divide by zero
ISR_NOERR 1   ; Debug
ISR_NOERR 2   ; Non-maskable interrupt
ISR_NOERR 3   ; Breakpoint
ISR_NOERR 4   ; Overflow
ISR_NOERR 5   ; Bound range exceeded
ISR_NOERR 6   ; Invalid opcode
ISR_NOERR 7   ; Device not available
ISR_ERR   8   ; Double fault
ISR_NOERR 9   ; Coprocessor segment overrun (reserved)
ISR_ERR   10  ; Invalid TSS
ISR_ERR   11  ; Segment not present
ISR_ERR   12  ; Stack-segment fault
ISR_ERR   13  ; General protection fault
ISR_ERR   14  ; Page fault
ISR_NOERR 15  ; Reserved
ISR_NOERR 16  ; x87 Floating Point
ISR_ERR   17  ; Alignment check
ISR_NOERR 18  ; Machine check
ISR_NOERR 19  ; SIMD floating-point
ISR_NOERR 20  ; Virtualization
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_NOERR 30
ISR_NOERR 31

; Shared stub: call C isr_handler(int_no)
isr_common_stub:
    pusha
    mov eax, esp          ; Pass pointer to stack frame if needed
    call isr_handler
    popa
    add esp, 8            ; Clean up pushed int_no and error code
    iretd
