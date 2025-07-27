#include "vmm_alloc_region.h"

void * __vmm_alloc_region(uint32_t num_pages, uint32_t flags) {
    uint32_t phys_base = __pmm_malloc(num_pages);
    if (!phys_base) {
        dbg_print_f("[KERNEL][VMM][ERR]: Failed to allocate %d pages for region.\n", num_pages);
        return NULL;
    }

    uint32_t base_virt = next_free_virtual;
    for (uint32_t i = 0; i < num_pages; i++) {
        uint32_t phys = phys_base + i * PAGE_SIZE;
        uint32_t virt = next_free_virtual;

        __vmm_map(virt, phys, flags);

        next_free_virtual += PAGE_SIZE;
    }
    return (void*) base_virt;
}