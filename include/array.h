#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

typedef struct array {
    size_t capacity;
    size_t length;
    size_t size;
    void *array;
} array_t;

array_t *array_init(size_t nitems, size_t size);
void array_free(array_t *array);
int array_set(array_t *array, size_t index, void *data);
int array_get(array_t *array, size_t index, void *data);
int array_insert(array_t *array, size_t index, void *data);
int array_remove(array_t *array, size_t index);
int array_push_back(array_t *array, void *data);
int array_pop_back(array_t *array);

#endif
