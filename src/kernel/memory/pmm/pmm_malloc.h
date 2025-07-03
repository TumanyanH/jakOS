#ifndef PMM_MALLOC_H
#define PMM_MALLOC_H

#include "pmm.h" 

#define MAX_SEG 32

pmm_segment_t pool[MAX_SEG];
int pool_index = 0;

void pmm_malloc(uint32_t size);

#endif