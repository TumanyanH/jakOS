#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include "vmm.h"

typedef struct vmm_table {
    uint32_t    start;
    uint32_t    pages_count;
    int         free_bytes;
} vmm_table_t;

uint32_t        next_virt_addr;
int             pages_count;
vmm_table_t     *exisiting_pages;

#include "../src/kernel/memory/heap/heap_init.h"
#include "../src/kernel/memory/heap/kmalloc.h"
#include "../src/kernel/memory/heap/kfree.h"

#endif
