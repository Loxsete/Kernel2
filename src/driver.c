#include <hal/driver.h>
#include <mem.h>
#include <stddef.h>

static driver_t* drivers_list = NULL;
static driver_t* last_driver = NULL;

driver_t* add_driver(driver_ops_t* ops){
    (void)ops;
    driver_t* driver = kalloc(sizeof(driver_t));
    if(driver == NULL){
        *(uint16_t*)0xb8000 = (0xf << 8) | '!'; // todo: replace with arch_panic
        while(1);
    }
    driver->ops = ops;
    driver->driver_type = driver->ops->type;
    driver->priv_state = driver->ops->new();

    if(last_driver == NULL){
        drivers_list = driver;
    } else {
        last_driver->next = driver;
    }
    last_driver = driver;

    return driver;
}

driver_t* find_first_driver(uint16_t type){
    (void)type;
    driver_t* driver = drivers_list;
    while(driver != NULL) {
        if(driver->driver_type == type)
            return driver;
        driver = driver->next;
    }
    return NULL;
}
