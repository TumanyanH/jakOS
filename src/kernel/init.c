#include "init.h"


extern uint32_t __end;

static void wcom_msg(){
    print("     __        __   ________    _________\n");
    print("    |__|____  |  | _\\_____  \\  /   _____/\n");
    print("    |  \\__  \\ |  |/ //   |   \\ \\_____  \\ \n");
    print("    |  |/ __ \\|    </    |    \\/        \\ \n");
    print("/\\__|  (____  /__|_ \\_______  /_______  /\n");
    print("\\______|    \\/     \\/       \\/        \\/ \n");
}

void k_init_general()
{
    clear_screen();
    wcom_msg();    
    gdt_init();
}
// uint32_t pci_config_read_dword(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
//     uint32_t address =
//         (1U << 31) |                   // enable bit
//         ((uint32_t)bus << 16) |
//         ((uint32_t)device << 11) |
//         ((uint32_t)function << 8) |
//         (offset & 0xFC);               // aligned

//     outb(0xCF8, address);
//     return inb(0xCFC);
// }

// uint16_t pci_config_read_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
//     uint32_t data = pci_config_read_dword(bus, device, function, offset);
//     return (offset & 2) ? (data >> 16) & 0xFFFF : data & 0xFFFF;
// }

// uint8_t pci_config_read_byte(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
//     uint32_t data = pci_config_read_dword(bus, device, function, offset);
//     return (data >> ((offset & 3) * 8)) & 0xFF;
// }

// void pci_scan_storage_devices(void) {
//     for (uint16_t bus = 0; bus < 256; bus++) {
//         for (uint8_t device = 0; device < 32; device++) {
//             for (uint8_t function = 0; function < 8; function++) {

//                 uint16_t vendor_id = pci_config_read_word(bus, device, function, 0x00);
//                 if (vendor_id == 0xFFFF)
//                     continue;

//                 uint8_t class_code = pci_config_read_byte(bus, device, function, 0x0B);
//                 uint8_t subclass   = pci_config_read_byte(bus, device, function, 0x0A);

//                 // Filter only storage class
//                 if (class_code != 0x01)
//                     continue;

//                 uint16_t device_id = pci_config_read_word(bus, device, function, 0x02);
//                 uint8_t prog_if = pci_config_read_byte(bus, device, function, 0x09);

//                 dbg_print_f("[PCI][STORAGE] Bus %d Device %d Function %d: Vendor %x Device %x Subclass %x ProgIF %x\n",
//                     bus, device, function, vendor_id, device_id, subclass, prog_if);

//                 // optional: read BAR0
//                 uint32_t bar0 = pci_config_read_dword(bus, device, function, 0x10);
//                 dbg_print_f("        BAR0 = %x\n", bar0);
//             }
//         }
//     }
// }
void k_init_memory(multiboot_info_t* mboot)
{
    print_f("The start of PHYMEM is %x \n", &__end);
    pmm_segment_t *pmm_seg = __pmm_mem_init(
        mboot,
        &__end
    );
    __vmm_mem_init();
    __heap_init();

    // pci_scan_storage_devices();
}

void k_init_keyboard()
{   
    pic_remap();
    idt_init();
    keyboard_install();
}
