#ifndef VMM_MAP_H
#define VMM_MAP_H

#include "pmm.h"
#include "vmm.h"

void __vmm_map(uint32_t virt, uint32_t phys, uint32_t flags);

#endif