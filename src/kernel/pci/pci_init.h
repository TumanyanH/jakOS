#ifndef PCI_SCAN_H
#define PCI_SCAN_H

#include "pci.h"

void set_pci_bus_options(uint8_t bus, uint8_t class_code, uint8_t subclass, uint8_t device, uint8_t function );
void __pci_init();

#endif