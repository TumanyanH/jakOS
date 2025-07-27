#include "pmm_malloc.h"

uint32_t next_kernel_struct_phys = 0x00100000;

void* __pmm_alloc_lowmem_page() {
    uint32_t addr = (next_kernel_struct_phys + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1); // align to 4KB
    if (addr >= 0x00400000) {
        dbg_print_f("[PMM][ERR]: Out of low memory (<4MB)!\n");
        while(1);
    }
    next_kernel_struct_phys = addr + PAGE_SIZE;
    return (void*) addr;
}

/**
 * finds the segments which are applicable
 * 
 * @return single pointer (pmm_segment_t *) 
 * with next member set to NULL if given size
 * fits inside a single segment
 * 
 * @return multiple pointers (pmm_segment_t *)
 * if given size fits inside different segments
 */
static pmm_segment_t *find_applicable_segment(uint32_t size) {
    pmm_segment_t *iter = __pmm_g;
    pmm_segment_t *res_head = NULL;
    pmm_segment_t *res_tail = NULL;

    uint32_t collected = 0;

    while (iter && collected < size) {
        if (iter->available_pages == 0) {
            iter = iter->next;
            continue;
        }

        if (!res_head) {
            res_head = iter;
            res_tail = iter;
        } else {
            res_tail->next = iter;
            res_tail = iter;
        }

        uint32_t to_consume = size - collected;
        if (iter->available_pages > to_consume) {
            iter->available_pages -= to_consume;
            collected += to_consume;
        } else {
            collected += iter->available_pages;
            iter->available_pages = 0;
        }

        iter = iter->next;
    }

    if (res_tail) res_tail->next = NULL;

    if (collected >= size) {
        return res_head;
    } else {
        return NULL;
    }
}

void print_alloc_table () {
    dbg_print_f("-------------ALLOCATION TABLE-------------\n");
    pmm_alloc_t *iter = __pmm_alloc_table_g;

    while (iter)
    {
        dbg_print_f("PID %d\n", iter->pid);
        dbg_print_f("SEGMENT ADDR %x\n", iter->segment->seg->addr);
        dbg_print_f("ALLOCATION LEN %d\n", iter->pages_allocated);
        dbg_print_f("OFFSET %d\n", iter->segment_page_offset);
        dbg_print_f("+++++++++++++++++++++++++++++++++++\n");
        iter = iter->next;
    }
    dbg_print_f("-----------ALLOCATION TABLE END------------\n");
}

uint32_t seg_find_free(pmm_segment_t *seg, uint32_t *collected, uint32_t size) {
    uint8_t *bm_start = seg->bm_start;

    uint32_t offset = 0;
    int found = 0;
    int count = 0;
    while (bm_start < seg->bm_end) {
        for (int i = 0; i < 8; i++)
        {
            if (((*bm_start >> i) & 1) == 0)
            {
                if (found == 0) offset = ((bm_start - seg->bm_start) * 8) + i; 
                found = 1;
                (*collected)++;
                count++;
            }
            if ((*collected) == size || (((*bm_start >> i) & 1) && found == 1))
            {
                return offset;
            }
        }
        bm_start++;
    }
    return 0;
}

void mark_used(pmm_segment_t *seg, uint32_t offset, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        uint32_t bit_index = offset + i;
        uint32_t byte_index = bit_index / 8;
        uint32_t bit_in_byte = bit_index % 8;
        seg->bm_start[byte_index] |= (1 << bit_in_byte);
    }
}

uint64_t alloc_space(pmm_segment_t* using_segments, uint32_t size) {
    uint32_t collected = 0;
    pmm_segment_t *iter = using_segments;
    uint64_t start_addr = iter->seg->addr;
    pmm_alloc_t *alloc = alloc_alloc();
    int first_iter = 1;

    while (iter && collected < size) {
        uint32_t offset = seg_find_free(iter, &collected, size);
        if (first_iter) {
            start_addr += (uint64_t)offset * PAGE_SIZE;
            first_iter = 0;
        }
        mark_used(iter, offset, size);
        
        
        alloc->pages_allocated = size;
        alloc->segment = iter;
        alloc->segment_page_offset = offset;
        alloc->next = NULL;
        
        if (!__pmm_alloc_table_g) {
            __pmm_alloc_table_g = alloc;
        } else {
            pmm_alloc_t *table = __pmm_alloc_table_g;
            while (table->next)
                table = table->next;

            table->next = alloc;
        }
        iter = iter->next;
    }
    return start_addr;
}

/**
 * takes size in pages and changes inside bitmap all needed flagss
 * 
 * @param   size (uint32_t)
 * 
 * @return  address of allocated space (uint64_t)
 */
void *__pmm_malloc(uint32_t size) {
    pmm_segment_t *using_segments = find_applicable_segment(size);
    if (!using_segments) return NULL;

    uint64_t act_addr = alloc_space(using_segments, size);
    if (!act_addr) return NULL;

    return act_addr;
}
