#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>

#define array_t(T)       \
struct {                 \
    size_t capacity;     \
    size_t length;       \
    size_t element_size; \
    T *array;            \
}

typedef struct array_generic {
    size_t capacity;
    size_t length;
    size_t element_size;
    void *array;
} array_generic_t;

#define array(arr) (arr->array)

#define array_for_each(T, item, arr) \
    for (T item = array(arr); (uint8_t *)item < (uint8_t *)array(arr) + (arr->length * arr->element_size); item = (uint8_t *)item + arr->element_size)

#define array_init(T) _array_init(0, sizeof(T))
#define array_init_prealloc(T, nitems) _array_init(nitems, sizeof(T))
void *_array_init(size_t nitems, size_t element_size);

void array_free(void *array);

#define array_insert(T, array, index, data) _array_insert((array_generic_t *)array, index, &(T){data}, sizeof(T))
int _array_insert(array_generic_t *array, size_t index, void *data, size_t element_size);

#define array_remove(array, index) _array_remove((array_generic_t *)array, index)
int _array_remove(array_generic_t *array, size_t index);

#define array_append(T, arr, val) _array_append((array_generic_t *)arr, &(T){val}, sizeof(T))
int _array_append(array_generic_t *array, void *data, size_t element_size);

#endif
