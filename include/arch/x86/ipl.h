#pragma once

#include <stdint.h>
extern uint32_t boot_sig;
extern uint32_t boot_context;

void ipl(void);