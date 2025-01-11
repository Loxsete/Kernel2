#pragma once

#include <stdint.h>

typedef struct {
	void (*putc)(void* priv_state, const char ch);
	uint16_t (*read)(void* priv_state); // returns event code
} io_driver_ops_t;
