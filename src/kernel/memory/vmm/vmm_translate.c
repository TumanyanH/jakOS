#include "vmm_translate.h"

vaddr_parts_t split_vaddr(uint32_t virt_addr) {
    vaddr_parts_t parts;
    parts.pd_idx = virt_addr >> 22;
    parts.pt_idx = (virt_addr >> 12) & 0x3FF;
    parts.offset = virt_addr & 0xFFF;
    return parts;
}

void print_vaddr_parts(uint32_t virt_addr) {
    vaddr_parts_t p = split_vaddr(virt_addr);
    dbg_print_f("Virtual address: %x\n", virt_addr);
    dbg_print_f("  PDE index: %d\n", p.pd_idx);
    dbg_print_f("  PTE index: %d\n", p.pt_idx);
    dbg_print_f("  Offset:    %x\n", p.offset);
}