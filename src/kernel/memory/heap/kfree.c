#include "kfree.h"

int find_alloc(void *addr) {
    for (int i = 0; i < pages_count; i++) {
        uint32_t start = (uint32_t)exisiting_pages[i].start;
        uint32_t end   = start + exisiting_pages[i].pages_count * PAGE_SIZE;
        if ((uint32_t)addr >= start && (uint32_t)addr < end) {
            return i;  // found block containing addr
        }
    }
    return -1;  // not found
}

void kfree(void *addr) {
    int alloc_idx = find_alloc(addr);
    if (alloc_idx == -1) {
        dbg_print_f("[KERNEL][HEAP][KFREE]: Unknown address to free: %x\n", addr);
        return;
    }

    exisiting_pages[alloc_idx].free_bytes = exisiting_pages[alloc_idx].pages_count * PAGE_SIZE;

    dbg_print_f("[KERNEL][HEAP][KFREE]: Freed block at %x (now %d free bytes)\n",
                exisiting_pages[alloc_idx].start,
                exisiting_pages[alloc_idx].free_bytes);
}