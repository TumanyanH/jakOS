#include "init.h"

extern uint32_t __end;

static void wcom_msg(){
    print("     __        __   ________    _________\n");
    print("    |__|____  |  | _\\_____  \\  /   _____/\n");
    print("    |  \\__  \\ |  |/ //   |   \\ \\_____  \\ \n");
    print("    |  |/ __ \\|    </    |    \\/        \\ \n");
    print("/\\__|  (____  /__|_ \\_______  /_______  /\n");
    print("\\______|    \\/     \\/       \\/        \\/ \n");
}

void k_init_general()
{
    clear_screen();
    wcom_msg();    
    gdt_init();
}

void k_init_memory(multiboot_info_t* mboot)
{
    print_f("The start of PHYMEM is %x \n", &__end);
    __pmm_mem_init(
        mboot,
        __end
    );
}

void k_init_keyboard()
{   
    pic_remap();
    idt_init();
    keyboard_install();
}
