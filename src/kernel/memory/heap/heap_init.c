#include "heap_init.h"

static void nullify_list() {
    int i = 0;

    while (i < 1024) {
        exisiting_pages[i].start = NULL;
        exisiting_pages[i].pages_count = 0;
        exisiting_pages[i].free_bytes = 0;
        i++;
    }
}

void __heap_init() {
    pages_count = 0;
    next_virt_addr = KERNEL_BASE;
    exisiting_pages = (vmm_table_t *)__vmm_alloc_region((sizeof(vmm_table_t) * 1024) / PAGE_SIZE);
    nullify_list();
    dbg_print_f("[KERNEL][HEAP]: Allocation structure initialized at: %x\n", exisiting_pages);
}