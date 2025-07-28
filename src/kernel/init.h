#ifndef INIT_H
#define INIT_H

#include <stdint.h>
#include <stddef.h>
#include "io.h"
#include "isr.h"
#include "kbd.h"
#include "idt.h"
#include "multiboot.h"
#include "display.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"
#include "pci.h"

void k_init_general(void);
void k_init_memory(multiboot_info_t* mboot);
void k_init_keyboard(void);
void k_init_devices(void);

#endif