#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <array.h>

static array_generic_t *array_expand(array_generic_t *array)
{
    if (array->capacity == 0) {
        array->capacity = 16;
    } else {
        array->capacity <<= 1;
    }

    array->array = realloc(array->array, array->element_size * array->capacity);
    if (!array->array) {
        return NULL;
    }

    return array;
}

static array_generic_t *array_reduce(array_generic_t *array)
{
    array->capacity >>= 1;
    array->array = realloc(array->array, array->element_size * array->capacity);
    if (!array->array) {
        return NULL;
    }

    return array;
}

void *_array_init(size_t nitems, size_t element_size)
{
    array_generic_t *array = calloc(1, sizeof(array_generic_t));
    if (!array) {
        return NULL;
    }

    array->capacity = nitems;
    array->length = 0;
    array->element_size = element_size;

    array->array = calloc(nitems, element_size);
    if (!array->array){
        return NULL;
    }

    return array;
}

void array_free(void *array)
{
    if (!array) {
        return;
    }

    if (!((array_generic_t *)array)->array) {
        free(array);
        return;
    }

    free(((array_generic_t *)array)->array);
    free(array);
}

int _array_insert(array_generic_t *array, size_t index, void *data, size_t element_size)
{
    assert(array != NULL);
    assert(index < array->length);
    assert(data != NULL);
    assert(element_size == array->element_size);

    if (array->length == array->capacity) {
        array = array_expand(array);
        if (!array) {
            return -1;
        }
    }

    uint8_t *src = (uint8_t *)data;
    uint8_t *dest = ((uint8_t *)array->array) + array->element_size * index;
    uint8_t *tmp = calloc(array->length - index, array->element_size);
    if (!tmp) {
        return -1;
    }
    size_t shift_element_size = (array->element_size * array->length) - (array->element_size * index);
    memcpy(tmp, dest, shift_element_size);

    memcpy(dest+array->element_size, tmp, shift_element_size);
    free(tmp);

    memcpy(dest, src, array->element_size);
    array->length++;

    return 0;
}

int _array_remove(array_generic_t *array, size_t index)
{
    assert(array != NULL);
    assert(index < array->length);

    if (!array || index > array->length) {
        return -1;
    }

    uint8_t *dest = ((uint8_t *)array->array) + index * array->element_size;
    size_t shift_element_size = (array->element_size * array->length) - (array->element_size * index);

    memcpy(dest, dest+array->element_size, shift_element_size);
    array->length--;

    if (array->length < array->capacity >> 1) {
        array = array_reduce(array);
        if (!array) {
            return -1;
        }
    }

    return 0;
}

int _array_append(array_generic_t *array, void *data, size_t element_size)
{
    assert(array != NULL);
    assert(data != NULL);
    assert(element_size == array->element_size);

    if (array->length == array->capacity) {
        array = array_expand(array);
        if (!array) {
            return -1;
        }
    }

    uint8_t *src = (uint8_t *)data;
    uint8_t *dest = ((uint8_t *)array->array) + array->length * array->element_size;

    memcpy(dest, src, array->element_size);
    array->length++;

    return 0;
}
