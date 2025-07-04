#include "pmm_malloc.h"

pmm_segment_t pool[MAX_SEGMENTS];
int pool_index = 0;

/**
 * helper function since there
 * is no any allocation functionality
 */
pmm_segment_t *alloc_inside_arr() {
    if (pool_index >= MAX_SEGMENTS) return NULL;
    return &pool[pool_index++];
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

        pmm_segment_t *new_seg = alloc_inside_arr();
        *new_seg = *iter;
        new_seg->next = NULL;

        if (!res_head) {
            res_head = new_seg;
            res_tail = new_seg;
        } else {
            res_tail->next = new_seg;
            res_tail = res_tail->next;
        }

        if (iter->available_pages > (size - collected)) collected += size - collected;
        else collected += iter->available_pages;

        iter = iter->next;
    }

    if (collected >= size) {
        return res_head;
    } else {
        return NULL;
    }
}

/**
 * changes memory status on the bitmap 
 * accordingly to used pages
 * 
 * @param   using_segments (pmm_segment_t pointer to selected segements)
 * @param   size (uint32_t)
 */
void change_memory_status (pmm_segment_t *using_segments, uint32_t size) {
    pmm_segment_t *iter = using_segments;
    uint32_t collected = 0;

    while (iter) {
        uint32_t seg_alloc_size = 
            (iter->available_pages > (size - collected) 
                ? (size - collected)
                : iter->available_pages );
        collected += seg_alloc_size; 
        iter->available_pages -= seg_alloc_size;

        uint32_t iter_count = 0;
        while (iter_count < seg_alloc_size / 8 ) {
            
            *(iter->bm_start + iter_count) = 0xFF;
            iter_count++;
        }
        uint32_t i = 0;
        while (i < seg_alloc_size % 8) {
            iter->bm_start[iter_count] |= (1 << i);
            i++;
        }
        
        iter = iter->next;
    }
}

void print_bitmap(pmm_segment_t *seg) {
    dbg_print_f("Bitmap for segment at %d:\n", (uint32_t)seg);

    uint8_t *start = seg->bm_start;
    uint8_t *end   = seg->bm_end;

    int byte_index = 0;
    while (start < end) {
        dbg_print_f("Byte %d: ", byte_index);
        for (int bit = 7; bit >= 0; bit--) {
            int bit_value = (*start >> bit) & 1;
            dbg_print_f("%d", bit_value);
        }
        dbg_print_f("\n");
        start++;
        byte_index++;
    }

    dbg_print_f("End of bitmap.\n");
}

/**
 * takes size in pages and changes inside bitmap all needed flagss
 * 
 * @param   size (uint32_t)
 */
pmm_segment_t *__pmm_malloc(uint32_t size) {
    pmm_segment_t *using_segments = find_applicable_segment(size);
    
    // pmm_segment_t *iter = using_segments;
    // dbg_print_f("---- Found Memory Segments ----\n");
    // while (iter) {
    //     dbg_print_f("Segment:\n");
    //     dbg_print_f("  seg_addr:  %x\n", (uint32_t)iter);
    //     dbg_print_f("  mmap addr: %x\n", (uint32_t)iter->seg->addr);
    //     dbg_print_f("  mmap len:  %x\n", (uint32_t)iter->seg->len);
    //     dbg_print_f("  type:      %d\n", iter->seg->type);
    //     dbg_print_f("  pages:     %d\n", iter->count_pages);
    //     dbg_print_f("  bitmap:    start=%x end=%x\n", (uint32_t)iter->bm_start, (uint32_t)iter->bm_end);
    //     dbg_print_f("  next:      %x\n", (uint32_t)iter->next);
    //     iter = iter->next;
    // }
    // dbg_print_f("---- End Found Memory Segments ----\n");
    
    change_memory_status(using_segments, size);
    return using_segments;
}
