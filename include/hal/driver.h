#pragma once

#include <stdint.h>

#define DRIVER_TYPE_UNKNOWN 0
#define DRIVER_TYPE_IO 1
#define DRIVER_TYPE_MAX 2

typedef struct {
	void* (*new)(void); // returns new priv_state
	uint16_t type; // driver type
	void* type_ops; // driver ops
} driver_ops_t;

typedef struct _driver_t {
	driver_ops_t* ops;
	uint16_t driver_type;
	void* priv_state;
	struct _driver_t* next;
} driver_t;

driver_t* add_driver(driver_ops_t* ops);
driver_t* find_first_driver(uint16_t type);

