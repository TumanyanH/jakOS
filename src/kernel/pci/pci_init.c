#include "pci_init.h"

void enable_bus_master (
    uint8_t bus,
    uint8_t device, 
    uint8_t function ) {
    uint16_t cmd = pci_config_read_word(bus, device, function, 0x04);
    cmd |= (1 << 2);
    pci_config_write_word(bus, device, function, 0x04, cmd);
}

void set_pci_bus_options(
    uint8_t class_code,
    uint8_t subclass,
    uint8_t bus,
    uint8_t device, 
    uint8_t function ) {
        
    // network adapter
    if (class_code == 0x02 && subclass == 0x00) {
        enable_bus_master(bus, device, function);
        dbg_print_f("[DEVICE][NIC]: Bus master enabled!\n");
        __pci_init();
    }
}

void __pci_init() {
    nic_init ();

}