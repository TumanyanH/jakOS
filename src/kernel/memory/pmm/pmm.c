#include "pmm.h"

pmm_segment_t* __pmm_g = NULL;

void pmm_debug_print(void) {
    pmm_segment_t* seg = __pmm_g;

    dbg_print_f("---- Physical Memory Map ----\n");
    while (seg) {
        dbg_print_f("Segment:\n");
        dbg_print_f("  seg_addr:  %x\n", (uint32_t)seg);
        dbg_print_f("  mmap addr: %x\n", (uint32_t)seg->seg->addr);
        dbg_print_f("  mmap len:  %x\n", (uint32_t)seg->seg->len);
        dbg_print_f("  type:      %d\n", seg->seg->type);
        dbg_print_f("  pages:     %d\n", seg->count_pages);
        dbg_print_f("  bitmap:    start=%x end=%x\n", (uint32_t)seg->bm_start, (uint32_t)seg->bm_end);
        dbg_print_f("  next:      %x\n", (uint32_t)seg->next);

        seg = seg->next;
    }
    dbg_print_f("---- End of Memory Map ----\n");
}

static void pmm_seg_bitmap(multiboot_mmap_entry_t *mmap) {
    if (segment_index >= MAX_SEGMENTS) {
        dbg_print_f("[KERNEL][MEMORY]: Segments are too much!");
        return ;
    }
    uint32_t pages_count = (uint32_t)(mmap->len / 0x1000);
    uint32_t bitmap_bytes = (pages_count + 7) / 8;
    pmm_segment_t *seg = &segment_pool[segment_index++];

    uint8_t *bitmap_start = bitmap;

    for (uint32_t i = 0; i < bitmap_bytes; i++) {
        bitmap[i] = 0x00;
    }

    seg->bm_start        = bitmap_start;
    seg->bm_end          = bitmap + bitmap_bytes;
    seg->seg             = mmap;
    seg->count_pages     = pages_count;
    seg->available_pages = pages_count;
    seg->next            = NULL;

    if (!__pmm_g) {
        __pmm_g = seg;
    } else {
        pmm_segment_t *iter = __pmm_g;
        while (iter->next)
            iter = iter->next;

        iter->next = seg;
    }

    __pmm_total_pages += pages_count;
    bitmap += bitmap_bytes;
}

void __pmm_mem_init(multiboot_info_t* mboot, uint32_t __k_end) {
    dbg_print_f("[KERNEL][MEMORY]: Initializing... \n");

    if (!(mboot->flags & MBOOT_OK_FLAG)) {
        dbg_print_f("[KERNEL][MEMORY]: No available memory \n");
        return ;
    }

    multiboot_mmap_entry_t* mmap = (multiboot_mmap_entry_t*) mboot->mmap_addr;
    uint32_t mmap_end = mboot->mmap_addr + mboot->mmap_length;

    while ((uint32_t)mmap < mmap_end) {  
        if (mmap->type == 1) {
            dbg_print_f("[KERNEL][MEMORY]: Available segment %x of length %d\n",
                (uint32_t)mmap->addr,
                (uint32_t)mmap->len);

            pmm_seg_bitmap(mmap);
        }
        mmap = (multiboot_mmap_entry_t*)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
    }

    pmm_debug_print();

    // pmm_malloc((uint32_t)10);

}