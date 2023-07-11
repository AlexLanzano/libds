#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

typedef struct array {
    size_t capacity;
    size_t length;
    size_t element_size;
    void *array;
} array_t;

array_t *array_init(size_t nitems, size_t element_size);
void array_free(array_t *array);
int array_set(array_t *array, size_t index, void *data, size_t element_size);
int array_get(array_t *array, size_t index, void *data, size_t element_size);
int array_get_reference(array_t *array, size_t index, void *reference, size_t element_size);
int array_insert(array_t *array, size_t index, void *data, size_t element_size);
int array_remove(array_t *array, size_t index, size_t element_size);
int array_push_back(array_t *array, void *data, size_t element_size);
int array_pop_back(array_t *array, void *data, size_t element_size);

#endif
