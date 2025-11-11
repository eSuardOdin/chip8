#include "../inc/memory.h"

void* reallocate(void* pointer, size_t new_size) {
    if(new_size == 0) {
        free(pointer);
        return NULL;
    }
    void* ret = realloc(pointer, new_size);

    if(ret == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }

    return ret;
}