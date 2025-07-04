#ifndef PMM_INIT_H
#define PMM_INIT_H

#include <stddef.h>
#include <stdint.h>

#include "pmm.h"

#define PAGE_SIZE       0x1000
#define MAX_SEGMENTS    32
#define MBOOT_OK_FLAG   0x40



// extern pmm_segment_t *__pmm_g;
// extern pmm_segment_t segment_pool[MAX_SEGMENTS];
// extern int segment_index;

// uint8_t* bitmap;
// uint32_t __pmm_total_pages;

void pmm_debug_print();

#endif