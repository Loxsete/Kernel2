#include "hal/driver.h"
#include <stdio.h>
#include <stddef.h>
#include <hal/stdio.h>

static driver_t* io_driver = NULL;

void kputs(const char* str){
    while(*str)
        kputc(*str++);
}

void kputc(const char ch){
    if(io_driver == NULL)
        io_driver = find_first_driver(DRIVER_TYPE_IO);
    if(io_driver != NULL){
        io_driver_ops_t* io_ops = io_driver->ops->type_ops;
        io_ops->putc(io_ops, ch);
    }
}
