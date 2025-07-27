#ifndef VMM_H
#define VMM_H

#include <stddef.h>
#include <stdint.h>
#include "pmm.h"

// VMM general definitions
#define DIR_ENTRIES             1024
#define TABLE_ENTRIES           1024
#define PAGE_ENTRIES            1024
#define KERNEL_BASE             0xC0000000

// Flags for page table entries
#define VMM_FLAG_PRESENT        0x001
#define VMM_FLAG_RW             0x002
#define VMM_FLAG_USER           0x004

extern uint32_t next_free_virtual;

uint32_t* page_directory __attribute__((aligned(PAGE_SIZE)));
uint32_t* first_page_table __attribute__((aligned(PAGE_SIZE)));

#include "../src/kernel/memory/vmm/vmm_init.h"
#include "../src/kernel/memory/vmm/vmm_translate.h"

#endif