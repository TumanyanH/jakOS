#ifndef PMM_H
#define PMM_H

#include <stddef.h>
#include <stdint.h>

// #include "pmm_malloc.h"
#define MBOOT_OK_FLAG 0x40
#define MAX_SEGMENTS 8

typedef struct multiboot_mmap_entry {
    uint32_t size;
    uint64_t addr;
    uint64_t len;
    uint32_t type;
} __attribute__((packed)) multiboot_mmap_entry_t;

typedef struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
} __attribute__((packed)) multiboot_info_t;

typedef struct {
    uint8_t* bm_start;
    uint8_t* bm_end;
    multiboot_mmap_entry_t* seg;
    uint32_t count_pages;
    uint32_t available_pages;
    struct pmm_segment* next;
} pmm_segment_t;


static pmm_segment_t segment_pool[MAX_SEGMENTS];
static int segment_index = 0;

extern pmm_segment_t *__pmm_g;

uint8_t* bitmap;
uint32_t __pmm_total_pages;

void pmm_debug_print();

#endif