#include "pmm_free.h"

void __pmm_free(uint64_t addr, uint32_t size) {
    pmm_segment_t *seg = find_pmm_seg_by_addr(addr);
    uint64_t dist = addr - seg->seg->addr;
    uint32_t pg_idx = (uint32_t)dist / PAGE_SIZE;
    uint32_t byte_idx = pg_idx / 8;
    uint8_t  bit_idx  = pg_idx % 8;

    uint8_t b = seg->bm_start[byte_idx];

    for (uint32_t i = 0; i < size; i++, pg_idx++) {
        uint32_t byte_idx = pg_idx / 8;
        uint8_t  bit_idx  = pg_idx % 8;

        seg->bm_start[byte_idx] &= ~(1 << bit_idx);
    }

    seg->available_pages += size;
}