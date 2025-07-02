#include "multiboot.h"
#include "init.h"


void kernel_main(multiboot_info_t* mb_info) {

    k_init_general();
    k_init_keyboard();
    k_init_memory(mb_info);
    __asm__ volatile ("sti");
    while (1) {
        __asm__ volatile ("hlt\n\t");
    }
}
