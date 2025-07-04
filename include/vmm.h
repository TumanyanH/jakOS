#ifndef VMM_H
#define VMM_H

#include <stddef.h>
#include <stdint.h>
#define VM_START 0xC0000000 
#include "pmm.h"

typedef struct {
    uint32_t start;
    uint32_t size;
    uint32_t 
} vmm_table_t;

#include "../src/kernel/memory/vmm/vmm_init.h"

#endif