#pragma once

#include <stdint.h>

extern char read_port(uint16_t port);
extern void write_port(uint16_t port, uint8_t data);
