#ifndef VMM_H
#define VMM_H

// #include "pmm.h"
#include <stddef.h>
#include <stdint.h>
#define VM_START 0xC0000000 

typedef struct {
    uint32_t start;
    uint32_t size;
    uint32_t 
} vmm_table_t;

#endif