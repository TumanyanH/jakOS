#include "pci_init.h"

pci_vendor_t known_vendors[];
pci_class_t pci_classes[];

void __pci_init() {
    pci_vendor_t known_vendors[] = {
        {0x8086, "Intel"},
        {0x10EC, "Realtek"},
        {0x1AF4, "Virtio"},
        {0x1B36, "QEMU"},
        {0xFFFF, NULL}
    };

    pci_class_t pci_classes[] = {
        {0x01, 0x06, "SATA controller"},
        {0x01, 0x08, "NVMe controller"},
        {0x02, 0x00, "Ethernet controller"},
        {0x03, 0x00, "VGA controller"},
        {0xFF, 0xFF, NULL}
    };
}