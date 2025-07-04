#include "vmm_init.h"

void __vmm_mem_init() {
    dbg_print_f("[KERNEL][VMM]: Initializing...\n");
    
    pmm_segment_t *iter = __pmm_malloc((uint32_t)100);
    __pmm_free(iter->seg->addr + 10 * PAGE_SIZE, 10);
}