#include "kmalloc.h"

static int find_applicable_page(size_t size) {
    int idx = 0;

    while (exisiting_pages[idx].start) {
        if (exisiting_pages[idx].free_bytes > size) {
            return idx;
        }
        idx++;    
    }
    return -1;
}

static int new_page_allocation (int count) {
    int idx = pages_count;
    exisiting_pages[pages_count].start = __vmm_alloc_region(count);
    exisiting_pages[pages_count].pages_count = count;
    exisiting_pages[pages_count].free_bytes = 0x1000 * count;
    dbg_print_f("%x - %d - %d\n", exisiting_pages[pages_count].start, exisiting_pages[pages_count].pages_count, exisiting_pages[pages_count].free_bytes);
    pages_count += count;
    return idx;
}

static void change_page_status(int applic_idx, size_t size) {
    int count = size / PAGE_SIZE + 1;
    int i = 0;
    
    while (i < count) {
        if (i == count - 1)
            exisiting_pages[applic_idx + i].free_bytes -= (size % PAGE_SIZE);
        else 
            exisiting_pages[pages_count].free_bytes = count * PAGE_SIZE;
        i++;
    }
}

void *kmalloc(size_t size) {
    size_t needed_pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    int applic_idx = -1;

    if (size <= PAGE_SIZE) {
        applic_idx = find_applicable_page(size);
    }

    if (applic_idx == -1) {
        applic_idx = new_page_allocation(needed_pages);
    }

    void *addr = exisiting_pages[applic_idx].start + 
                 (exisiting_pages[applic_idx].pages_count * PAGE_SIZE - exisiting_pages[applic_idx].free_bytes);

    if (size <= PAGE_SIZE) {
        exisiting_pages[applic_idx].free_bytes -= size;
    } else {
        for (size_t i = 0; i < needed_pages; i++) {
            exisiting_pages[applic_idx + i].free_bytes = 0;
        }
    }

    dbg_print_f("[KERNEL][HEAP][KMALLOC]: Allocated %d bytes at virtual address %x\n", size, addr);
    return (void *)addr;
}