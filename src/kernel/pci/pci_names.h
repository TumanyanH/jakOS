#ifndef PCI_NAMES_H
#define PCI_NAMES_H

#include <stdint.h>

typedef struct {
    uint16_t vendor_id;
    const char *vendor_name;
} pci_vendor_t;

typedef struct {
    uint8_t class_code;
    uint8_t subclass;
    const char *desc;
} pci_class_t;

extern pci_vendor_t known_vendors[];
extern pci_class_t pci_classes[];

#endif