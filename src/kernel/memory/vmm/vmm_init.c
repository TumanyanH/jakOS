#include "vmm_init.h"

#define PAGE_SIZE 4096
#define DIR_ENTRIES 1024
#define PAGE_ENTRIES 1024
#define KERNEL_BASE 0xC0000000

uint32_t next_free_virtual = KERNEL_BASE + 0x01000000; // example next virtual start after kernel base

uint32_t *page_directory;
uint32_t *first_page_table;
uint32_t *second_page_table;


void __vmm_mem_init() {
    dbg_print_f("[KERNEL][VMM]: Initializing paging...\n");

    uint32_t pd_phys     = __pmm_malloc(1);
    uint32_t pt1_phys    = __pmm_malloc(1);
    uint32_t pt2_phys    = __pmm_malloc(1);

    page_directory   = (uint32_t*) pd_phys;
    first_page_table = (uint32_t*) pt1_phys;
    second_page_table= (uint32_t*) pt2_phys;

    for (uint32_t i = 0; i < DIR_ENTRIES; i++)
        page_directory[i] = 0;
    for (uint32_t i = 0; i < PAGE_ENTRIES; i++) {
        first_page_table[i]  = 0;
        second_page_table[i] = 0;
    }

    for (uint32_t i = 0; i < PAGE_ENTRIES; i++)
        first_page_table[i] = (i * PAGE_SIZE) | 3;

    for (uint32_t i = 0; i < PAGE_ENTRIES; i++)
        second_page_table[i] = (0x00400000 + i * PAGE_SIZE) | 3;

    page_directory[0] = pt1_phys | 3;
    page_directory[1] = pt2_phys | 3;

    uint32_t kernel_pd_idx = KERNEL_BASE >> 22;
    page_directory[kernel_pd_idx]     = pt1_phys | 3;
    page_directory[kernel_pd_idx + 1] = pt2_phys | 3;

    dbg_print_f("[KERNEL][VMM]: Page directory at phys=%x\n", pd_phys);

    asm volatile("mov %0, %%cr3" :: "r"(pd_phys));

    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" :: "r"(cr0));

    dbg_print_f("[KERNEL][VMM]: Paging enabled!\n");
}