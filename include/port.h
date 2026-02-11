#ifndef PORT_H
#define PORT_H

#include <stdint.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t val);

void outl(uint16_t port, uint32_t val);

uint32_t inl(uint16_t port);

#endif
