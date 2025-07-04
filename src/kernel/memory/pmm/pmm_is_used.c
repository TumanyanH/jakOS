#include "pmm_is_used.h"

/**
 * Find the segment where the 
 * given address  is part of
 * 
 * @param   addr uint64_t
 * 
 * @return  pmm_segment_t *
 */
pmm_segment_t *find_pmm_seg_by_addr(uint64_t addr) {
    pmm_segment_t *iter = __pmm_g;
    while (iter) {
        if (addr >= iter->seg->addr && addr < (iter->seg->addr + iter->seg->len))
            return iter;
        iter++;
    }
    return NULL;
}

/**
 * checks if the page is in use
 * 
 * @param   addr (pmm_segment_t* to segment object)
 * 
 * @return  int (true if it's used)
 */
int __pmm_is_used(uint64_t addr) {
    pmm_segment_t *seg = find_pmm_seg_by_addr(addr);
    uint64_t dist = addr - seg->seg->addr;
    uint32_t pg_idx = (uint32_t)dist / PAGE_SIZE;
    uint32_t byte_idx = pg_idx / 8;
    uint8_t  bit_idx  = pg_idx % 8;

    uint8_t b = seg->bm_start[byte_idx];

    return ((b >> bit_idx) & 1) != 0;
}