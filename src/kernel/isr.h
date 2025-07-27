#ifndef ISR_H
#define ISR_H

#include <stdint.h>

struct regs {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no;
    uint32_t err_code;
};

void isr_install();
void isr_handler(struct regs* r);

#endif
