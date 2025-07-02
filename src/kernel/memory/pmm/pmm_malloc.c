#include "pmm_malloc.h"

/**
 * finds the segments which are applicable
 * 
 * @return single pointer (pmm_segment_t *) 
 * with next member set to NULL if given size
 * fits inside a single segment
 * 
 * @return multiple pointers (pmm_segment_t *)
 * if given size fits inside different segments
 * 
 */
static pmm_segment_t *find_applicable_segment(uint32_t size) {
    pmm_segment_t *iter = __pmm_g;
    pmm_segment_t *res = NULL;

    uint32_t need = size;

    while (iter || need != 0) {
        if (iter->available_pages <= size) {
            need -= size;
            *res = *iter;
            res->next = NULL;
            return res;
        } else {
            need -= iter->availabe_pages;
            if (res) 
                *res = *iter;
                res->next = NULL;
            else
            {
                while (res->next) {
                    res = res->next;
                }
                *res = *iter;
                res->next = NULL;
            }
        }
        iter = iter->next;
    }
    return res;
}

/**
 * changes memory status on the bitmap 
 * accordingly to used pages
 */
void change_memory_status (pmm_segment_t *using_segments, uint32_t size) {
    // need to change the free flags inside
    // bitmap to used (0 to 1)
    // also another function should be 
    // implemented for gathering all the phys memory
    // pointers and pass them to vmm 
}



/**
 * takes size in pages and changes inside bitmap all needed flagss
 */
void pmm_malloc(uint32_t size) {
    pmm_segment_t *using_segments = find_allocable_segment(size);
    change_memory_status(using_segments, size);
}
