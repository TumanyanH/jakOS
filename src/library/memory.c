#include "memory.h"

#include <stddef.h>

static uint8_t *heap = (uint8_t*)0x100000;  // Start heap at 1MB
static size_t heap_size = 0;

void heap_init() {
    
};

void *kmalloc(size_t size) {
    void *mem = (void*)heap;
    heap += size;
    heap_size += size;
    return mem;
}

void *memset(void *dest, int val, size_t len) {
    unsigned char *ptr = dest;
    while (len-- > 0) {
        *ptr++ = (unsigned char) val;
    }
    return dest;
}

void *memcpy(void *dest, const void *src, size_t len) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (len--) {
        *d++ = *s++;
    }
    return dest;
}

void *memmove(void *dest, const void *src, size_t len) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    if (s < d && d < s + len) {
        d += len;
        s += len;
        while (len--) {
            *--d = *--s;
        }
    } else {
        while (len--) {
            *d++ = *s++;
        }
    }
    return dest;
}