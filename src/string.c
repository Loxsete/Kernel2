#include <string.h>

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t* dest_ = dest;
    const uint8_t* src_ = src;
    for(size_t i = 0; i < n; i++){
        if(n - i >= 4){
            *(uint32_t*)dest_ = *(uint32_t*)src_;
            dest_ += 4;
            src_ += 4;
            i+=3;
        } else {
            *dest_++ = *src_++;
        }
    }
    return dest;
}
