#include "pci_scan.h"

pci_vendor_t known_vendors[] = {
    {0x8086, "Intel Corporation"},
    {0x10EC, "Realtek Semiconductor"},
    {0x1AF4, "Virtio"},
    {0x1B36, "QEMU"},
    {0x1234, "Netwide Assembler"},
    {0x14E4, "Broadcom Inc."},
    {0x1002, "Advanced Micro Devices, Inc. [AMD/ATI]"},
    {0x10DE, "NVIDIA Corporation"},
    {0x1969, "VMware, Inc."},
    {0x15AD, "VMware, Inc."},
    {0x8087, "Intel Corporation"},
    {0x8087, "Intel Corporation"},
    {0x1033, "NEC Corporation"},
    {0x15AD, "VMware"},
    {0x8086, "Intel"},
    {0x14E4, "Broadcom"},
    {0x8086, "Intel Corporation"},
    {0x0E11, "Compaq"},
    {0x10B5, "Apple Inc."},
    {0x12D8, "Advanced Micro Devices"},
    {0x15AD, "VMware"},
    {0x0000, NULL} // terminator
};

pci_class_t pci_classes[] = {
    {0x00, 0x00, "Unclassified device"},
    
    // Mass Storage Controllers
    {0x01, 0x00, "SCSI storage controller"},
    {0x01, 0x01, "IDE storage controller"},
    {0x01, 0x02, "Floppy disk controller"},
    {0x01, 0x03, "IPI bus controller"},
    {0x01, 0x04, "RAID controller"},
    {0x01, 0x05, "ATA controller"},
    {0x01, 0x06, "SATA controller"},
    {0x01, 0x07, "Serial Attached SCSI (SAS) controller"},
    {0x01, 0x08, "Non-Volatile Memory controller (NVMe)"},
    {0x01, 0x80, "Other mass storage controller"},
    
    // Network Controllers
    {0x02, 0x00, "Ethernet controller"},
    {0x02, 0x01, "Token ring controller"},
    {0x02, 0x02, "FDDI controller"},
    {0x02, 0x03, "ATM controller"},
    {0x02, 0x04, "ISDN controller"},
    {0x02, 0x05, "WorldFip controller"},
    {0x02, 0x06, "PICMG 2.14 Multi Computing"},
    {0x02, 0x07, "Infiniband controller"},
    {0x02, 0x08, "Fabric controller"},
    
    // Display Controllers
    {0x03, 0x00, "VGA compatible controller"},
    {0x03, 0x01, "XGA controller"},
    {0x03, 0x02, "3D controller"},
    {0x03, 0x80, "Other display controller"},
    
    // Multimedia Controllers
    {0x04, 0x00, "Multimedia video controller"},
    {0x04, 0x01, "Multimedia audio controller"},
    {0x04, 0x02, "Computer telephony device"},
    {0x04, 0x03, "Audio device"},
    {0x04, 0x80, "Other multimedia controller"},
    
    // Memory Controllers
    {0x05, 0x00, "RAM memory"},
    {0x05, 0x01, "Flash memory"},
    {0x05, 0x80, "Other memory controller"},
    
    // Bridge Devices
    {0x06, 0x00, "Host bridge"},
    {0x06, 0x01, "ISA bridge"},
    {0x06, 0x02, "EISA bridge"},
    {0x06, 0x03, "MCA bridge"},
    {0x06, 0x04, "PCI-to-PCI bridge"},
    {0x06, 0x05, "PCMCIA bridge"},
    {0x06, 0x06, "NuBus bridge"},
    {0x06, 0x07, "CardBus bridge"},
    {0x06, 0x08, "RACEway bridge"},
    {0x06, 0x09, "PCI-to-PCI bridge (semi-transparent)"},
    {0x06, 0x80, "Other bridge device"},
    
    // Simple Communication Controllers
    {0x07, 0x00, "Serial controller"},
    {0x07, 0x01, "Parallel port"},
    {0x07, 0x02, "Multiport serial controller"},
    {0x07, 0x03, "Modem"},
    {0x07, 0x04, "IEEE 488.1/2 (GPIB) controller"},
    {0x07, 0x05, "Smart card"},
    {0x07, 0x80, "Other communication controller"},
    
    // Base System Peripheral
    {0x08, 0x00, "PIC"},
    {0x08, 0x01, "DMA controller"},
    {0x08, 0x02, "Timer"},
    {0x08, 0x03, "RTC controller"},
    {0x08, 0x04, "PCI hot-plug controller"},
    {0x08, 0x05, "SD host controller"},
    {0x08, 0x06, "IOMMU"},
    {0x08, 0x80, "Other system peripheral"},
    
    // Input Devices
    {0x09, 0x00, "Keyboard controller"},
    {0x09, 0x01, "Digitizer pen"},
    {0x09, 0x02, "Mouse controller"},
    {0x09, 0x03, "Scanner controller"},
    {0x09, 0x04, "Gameport controller"},
    {0x09, 0x80, "Other input controller"},
    
    // Docking Station
    {0x0A, 0x00, "Docking station"},
    
    // Processors
    {0x0B, 0x00, "Processor"},
    
    // Serial Bus Controllers
    {0x0C, 0x00, "FireWire (IEEE 1394) controller"},
    {0x0C, 0x01, "ACCESS bus controller"},
    {0x0C, 0x02, "SSA controller"},
    {0x0C, 0x03, "USB controller"},
    {0x0C, 0x04, "Fibre Channel controller"},
    {0x0C, 0x05, "SMBus controller"},
    {0x0C, 0x06, "InfiniBand controller"},
    {0x0C, 0x07, "IPMI Interface controller"},
    {0x0C, 0x08, "SERCOS controller"},
    {0x0C, 0x09, "CANbus controller"},
    
    // Wireless Controllers
    {0x0D, 0x00, "iRDA compatible controller"},
    {0x0D, 0x01, "Consumer IR controller"},
    {0x0D, 0x10, "RF controller"},
    {0x0D, 0x11, "Bluetooth controller"},
    {0x0D, 0x20, "Broadband controller"},
    {0x0D, 0x80, "Other wireless controller"},
    
    // Intelligent I/O Controllers
    {0x0E, 0x00, "I2O controller"},
    
    // Satellite Communication Controllers
    {0x0F, 0x01, "Satellite TV controller"},
    {0x0F, 0x02, "Satellite audio controller"},
    {0x0F, 0x03, "Satellite voice controller"},
    {0x0F, 0x04, "Satellite data controller"},
    
    // Encryption/Decryption Controllers
    {0x10, 0x00, "Network and computing encryption device"},
    
    // Data Acquisition and Signal Processing Controllers
    {0x11, 0x00, "DPIO modules"},
    {0x11, 0x01, "Performance counters"},
    {0x11, 0x10, "Communication synch. + time data"},
    {0x11, 0x20, "Management card"},
    
    {0xFF, 0xFF, NULL}
};

uint32_t pci_config_read_dword(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t address =
        (1U << 31) |                   // enable bit
        ((uint32_t)bus << 16) |
        ((uint32_t)device << 11) |
        ((uint32_t)function << 8) |
        (offset & 0xFC);               // aligned to dword

    outl(0xCF8, address);
    return inl(0xCFC);
}
uint16_t pci_config_read_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t data = pci_config_read_dword(bus, device, function, offset);
    return (offset & 2) ? (data >> 16) & 0xFFFF : data & 0xFFFF;
}

uint8_t pci_config_read_byte(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t data = pci_config_read_dword(bus, device, function, offset);
    return (data >> ((offset & 3) * 8)) & 0xFF;
}

static void print_pci_device(
    uint16_t vendor_id,
    uint8_t class_code,
    uint8_t subclass,
    uint16_t device_id,
    uint8_t prog_if )
{
    const char *vendor_name = "Unknown vendor";
    const char *class_desc = "Unknown device";

    for (int i = 0; known_vendors[i].vendor_id != 0xFFFF; i++) {
        if (known_vendors[i].vendor_id == vendor_id) {
            vendor_name = known_vendors[i].vendor_name;
            break;
        }
    }

    for (int i = 0; pci_classes[i].class_code != 0xFF; i++) {
        if (pci_classes[i].class_code == class_code &&
            pci_classes[i].subclass == subclass) {
            class_desc = pci_classes[i].desc;
            break;
        }
    }

    dbg_print_f("[PCI] %s %s - %x\n",
        vendor_name, class_desc, device_id);
}

void __pci_scan(void) {
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            for (uint8_t function = 0; function < 8; function++) {

                uint16_t vendor_id = pci_config_read_word(bus, device, function, 0x00);
                if (vendor_id == 0xFFFF)
                    continue;

                uint8_t class_code = pci_config_read_byte(bus, device, function, 0x0B);
                uint8_t subclass   = pci_config_read_byte(bus, device, function, 0x0A);

                // Filter only storage class
                if (class_code > 0x12)
                    continue;

                uint16_t device_id = pci_config_read_word(bus, device, function, 0x02);
                uint8_t prog_if = pci_config_read_byte(bus, device, function, 0x09);

                print_pci_device(vendor_id, class_code, subclass, device_id, prog_if);
                // optional: read BAR0
                uint32_t bar0 = pci_config_read_dword(bus, device, function, 0x10);
                // dbg_print_f("        BAR0 = %x\n", bar0);
            }
        }
    }
}