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

void k_init_keyboard();
void k_init_general();

#endif