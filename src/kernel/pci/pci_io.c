#include "pci_io.h"

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

void pci_config_write_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint16_t value) {
    uint32_t address = (1U << 31)
        | ((uint32_t)bus << 16)
        | ((uint32_t)device << 11)
        | ((uint32_t)function << 8)
        | (offset & 0xFC);

    outl(0xCF8, address);

    uint32_t shift = (offset & 2) * 8;          // which half-word?
    uint32_t data = inl(0xCFC);                 // read current 32-bit
    data &= ~(0xFFFF << shift);                 // clear old 16 bits
    data |= ((uint32_t)value << shift);         // insert new 16 bits

    outl(0xCFC, data);                          // write back
}