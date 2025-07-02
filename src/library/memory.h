#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

typedef struct {
    uint32_t base_low;
    uint32_t base_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
} __attribute__((packed)) memory_map_entry_t;

void *kmalloc(size_t size);
void *memset(void *dest, int val, size_t len);
void *memcpy(void *dest, const void *src, size_t len);
void *memmove(void *dest, const void *src, size_t len);

#endif