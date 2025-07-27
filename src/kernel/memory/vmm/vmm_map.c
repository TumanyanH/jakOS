#include "vmm_map.h"

void __vmm_map(uint32_t virt_addr, uint32_t phys_addr, uint32_t flags) {
    uint32_t pd_idx = virt_addr >> 22;
    uint32_t pt_idx = (virt_addr >> 12) & 0x3FF;

    uint32_t *page_table;

    if (!(page_directory[pd_idx] & 0x1)) {
        uint32_t new_pt_phys = __pmm_malloc(1);
        uint32_t *new_pt_virt = (uint32_t*) new_pt_phys;
        for (uint32_t i = 0; i < PAGE_ENTRIES; i++)
            new_pt_virt[i] = 0;

        page_directory[pd_idx] = new_pt_phys | flags | 0x1;
        page_table = new_pt_virt;
    } else {
        uint32_t pt_phys = page_directory[pd_idx] & 0xFFFFF000;
        page_table = (uint32_t*) pt_phys;
    }

    page_table[pt_idx] = (phys_addr & 0xFFFFF000) | (flags & 0xFFF) | 0x1;

    asm volatile ("invlpg (%0)" :: "r" (virt_addr) : "memory");
}