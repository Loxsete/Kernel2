#include <mem.h>
#include <stddef.h>

// 16MB
static const uint32_t heap_size = 16*1024*1024;
static uint32_t used = 0;
static uint8_t heap[16*1024*1024];

void* kalloc(uint32_t size){
    // 16byte padding
    size = (size & ~0xf) + ((size & 0xf)!=0);

    if(size >= heap_size || heap_size - used  < size)
        return NULL;
    for(uint32_t i = 0; i < size; i++)
        heap[i] = 0;
    return (void*)(heap+size);
}
