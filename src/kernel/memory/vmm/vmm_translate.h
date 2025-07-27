#ifndef VMM_TRANSLATE_H
#define VMM_TRANSLATE_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t pd_idx;
    uint32_t pt_idx;
    uint32_t offset;
} vaddr_parts_t;

vaddr_parts_t split_vaddr(uint32_t virt_addr);
void print_vaddr_parts(uint32_t virt_addr);

#endif