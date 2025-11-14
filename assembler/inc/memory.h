#ifndef MEMORY_H
#define MEMORY_H
#include "common.h"




#define GROW_CAPACITY(capacity) \
    ((capacity) = (capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, new_capacity) \
    (type*)reallocate(pointer, sizeof(type) * (new_capacity))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, 0)

void* reallocate(void* pointer, size_t new_size);
#endif