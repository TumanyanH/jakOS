#include "vmm_unmap.h"

void __vmm_unmap(uint32_t virt_addr) {
    uint32_t pd_idx = virt_addr >> 22;
    uint32_t pt_idx = (virt_addr >> 12) & 0x3FF;

    if (!(page_directory[pd_idx] & VMM_FLAG_PRESENT)) {
        dbg_print_f("[KERNEL][VMM][ERR]: Tried to unmap, but page directory entry not present.\n");
        return;
    }

    uint32_t pt_phys = page_directory[pd_idx] & 0xFFFFF000;
    uint32_t* page_table = (uint32_t*) pt_phys;

    uint32_t old_entry = page_table[pt_idx];
    if (old_entry & VMM_FLAG_PRESENT) {
        uint32_t old_phys = old_entry & 0xFFFFF000;
        __pmm_free((void*)old_phys, 1);
        page_table[pt_idx] = 0;
        asm volatile("invlpg (%0)" :: "r" (virt_addr) : "memory");
    } else {
        dbg_print_f("[KERNEL][VMM][ERR]: Tried to unmap, but PTE not present.\n");
    }

    asm volatile("invlpg (%0)" :: "r" (virt_addr) : "memory");
}