#include "pmm_init.h"

pmm_segment_t segment_pool[MAX_SEGMENTS];
int segment_index = 0;

pmm_segment_t pool[MAX_SEGMENTS];
int pool_index = 0;

pmm_alloc_t pmm_alloc_pool[MAX_ALLOC_TABLE];
int alloc_index = 0;


pmm_segment_t* alloc_segment() {
    if (segment_index >= MAX_SEGMENTS) return NULL;
    return &segment_pool[segment_index++];
}

pmm_alloc_t* alloc_alloc() {
    if (alloc_index >= MAX_SEGMENTS) return NULL;
    return &pmm_alloc_pool[alloc_index++];
}

pmm_segment_t *alloc_inside_arr() {
    if (pool_index >= MAX_SEGMENTS) return NULL;
    return &pool[pool_index++];
}

pmm_alloc_t* __pmm_alloc_table_g = NULL;
pmm_segment_t * __pmm_g = NULL;

void pmm_debug_print(void) {
    pmm_segment_t* seg = __pmm_g;

    dbg_print_f("---- Physical Memory Map ----\n");
    while (seg) {
        dbg_print_f("Segment:\n");
        dbg_print_f("  seg_addr:  %x\n", (uint32_t)seg);
        dbg_print_f("  mmap addr: %x\n", (uint32_t)seg->seg->addr);
        dbg_print_f("  mmap len:  %x\n", (uint32_t)seg->seg->len);
        dbg_print_f("  type:      %d\n", seg->seg->type);
        dbg_print_f("  available: %d\n", seg->available_pages);
        dbg_print_f("  pages:     %d\n", seg->count_pages);
        dbg_print_f("  bitmap:    start=%x end=%x\n", (uint32_t)seg->bm_start, (uint32_t)seg->bm_end);
        dbg_print_f("  next:      %x\n", (uint32_t)seg->next);

        seg = seg->next;
    }
    dbg_print_f("---- End of Memory Map ----\n");
}

static void pmm_seg_bitmap(multiboot_mmap_entry_t *mmap) {
    if (segment_index >= MAX_SEGMENTS) {
        dbg_print_f("[KERNEL][PMM]: Segments are too much!");
        return ;
    }
    uint32_t pages_count = (uint32_t)(mmap->len / 0x1000);
    uint32_t bitmap_bytes = (pages_count + 7) / 8;
    pmm_segment_t *seg = alloc_segment();
    
    uint8_t *bitmap_start = 0; 
    if (!__pmm_g)
        bitmap_start = __usable;
    else {
        pmm_segment_t *iter = __pmm_g;
        while (iter->next)
            iter = iter->next;
        bitmap_start = iter->bm_end + 1;
    }


    for (uint32_t i = 0; i < bitmap_bytes; i++) {
        *(bitmap_start + i) = 0x00;
    }

    seg->bm_start        = bitmap_start;
    seg->bm_end          = bitmap_start + bitmap_bytes;
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

void bitmap_debug_print(uint8_t *start, uint8_t *end) {
    dbg_print_f("Bitmap for segment at %x:\n", (uint32_t)start);

    uint32_t byte_index = 0;
    int i = 0;

    while (start < end && i < 15) {
        dbg_print_f("Byte %d (%x): ", byte_index, (uint32_t)start);
        for (int bit = 7; bit >= 0; bit--) {
            dbg_print_f("%d", (*start >> bit) & 1);
        }
        dbg_print_f("\n");
        start++;
        byte_index++;
        i++;
    }

    dbg_print_f("End of bitmap.\n");
}

void __pmm_mem_init(mboot_info_t* mboot, uint32_t __k_end) {
    dbg_print_f("[KERNEL][PMM]: Initializing... \n");

    if (!(mboot->flags & MBOOT_OK_FLAG)) {
        dbg_print_f("[KERNEL][PMM]: No available memory \n");
        return ;
    }

    __usable = __k_end;
    dbg_print_f("[KERNEL][PMM]: Start of usable memory detected - %x\n", __usable);
    multiboot_mmap_entry_t* mmap = (multiboot_mmap_entry_t*) mboot->mmap_addr;
    uint32_t mmap_end = mboot->mmap_addr + mboot->mmap_length;

    while ((uint32_t)mmap < mmap_end) {  
        if (mmap->type == 1 && mmap->addr >= KERNEL_START) {
            dbg_print_f("[KERNEL][PMM]: Available segment %x of length %d\n",
                (uint32_t)mmap->addr,
                (uint32_t)mmap->len);
            if (mmap->addr == KERNEL_START && mmap->len >= 0x00400000) {
                mmap->addr = mmap->addr + 0x00400000;
                mmap->len = mmap->len - 0x00400000;
            }
            pmm_seg_bitmap(mmap);
        }
        mmap = (multiboot_mmap_entry_t*)((uint32_t)mmap + mmap->size + sizeof(mmap->size));
    }
    pmm_debug_print();
    // bitmap_debug_print(__pmm_g->bm_start, __pmm_g->bm_end);
}