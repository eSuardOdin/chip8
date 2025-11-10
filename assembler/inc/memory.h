#ifndef MEMORY_H
#define MEMORY_H
#include "common.h"

#define GROW_CAPACITY(capacity) \
    ((capacity) = (capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, pointer, old_capacity, new_capacity) \
    (type*)reallocate(pointer, sizeof(type) * (old_capacity), sizeof(type) * (new_capacity))

#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

void* reallocate(void* pointer, size_t old_size, size_t new_size);
#endif