#ifndef VMM_ALLOC_MAP_H
#define VMM_ALLOC_MAP_H

#include "vmm.h"

void *__vmm_alloc_region (uint32_t num_pages, uint32_t flags);

#endif