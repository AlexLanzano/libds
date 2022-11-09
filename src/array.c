#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <array.h>

static array_t *array_expand(array_t *array)
{
    array->capacity <<= 1;
    array->array = realloc(array->array, array->size * array->capacity);
    if (!array->array) {
        return NULL;
    }

    return array;
}

static array_t *array_reduce(array_t *array)
{
    array->capacity >>= 1;
    array->array = realloc(array->array, array->size * array->capacity);
    if (!array->array) {
        return NULL;
    }

    return array;
}

array_t *array_init(size_t nitems, size_t size)
{
    array_t *array = calloc(1, sizeof(array_t));
    if (!array) {
        return NULL;
    }

    array->capacity = nitems;
    array->length = 0;
    array->size = size;

    array->array = calloc(nitems, size);
    if (!array->array){
        return NULL;
    }

    return array;
}

void array_free(array_t *array)
{
    if (!array) {
        return;
    }

    if (!array->array) {
        free(array);
        return;
    }

    free(array->array);
    free(array);
}

int array_set(array_t *array, size_t index, void *data)
{
    assert(array);
    assert(data);

    if (index >= array->length) {
        return -1;
    }

    uint8_t *src = (uint8_t *)data;
    uint8_t *dest = ((uint8_t *)array->array) + array->size * index;

    memcpy(dest, src, array->size);
    return 0;
}

int array_get(array_t *array, size_t index, void *data)
{
    assert(array);
    assert(data);

    if (index >= array->length) {
        return -1;
    }

    uint8_t *src = ((uint8_t *)array->array) + array->size * index;
    uint8_t *dest = (uint8_t *)data;

    memcpy(dest, src, array->size);
    return 0;
}
int array_insert(array_t *array, size_t index, void *data)
{
    assert(array);
    assert(data);

    if (index > array->length) {
        return -1;
    }

    if (array->length == array->capacity) {
        array = array_expand(array);
        if (!array) {
            return -1;
        }
    }

    uint8_t *src = (uint8_t *)data;
    uint8_t *dest = ((uint8_t *)array->array) + array->size * index;
    uint8_t *tmp = calloc(array->length - index, array->size);
    if (!tmp) {
        return -1;
    }
    size_t shift_size = (array->size * array->length) - (array->size * index);
    memcpy(tmp, dest, shift_size);

    memcpy(dest+array->size, tmp, shift_size);
    free(tmp);

    memcpy(dest, src, array->size);
    array->length++;

    return 0;
}

int array_remove(array_t *array, size_t index)
{
    assert(array);

    if (index > array->length) {
        return -1;
    }

    uint8_t *dest = ((uint8_t *)array->array) + index * array->size;
    size_t shift_size = (array->size * array->length) - (array->size * index);

    memcpy(dest, dest+1, shift_size);
    array->length--;

    if (array->length < array->capacity >> 1) {
        array = array_reduce(array);
        if (!array) {
            return -1;
        }
    }

    return 0;
}

int array_push_back(array_t *array, void *data)
{
    assert(array);
    assert(data);

    if (array->length == array->capacity) {
        array = array_expand(array);
        if (!array) {
            return -1;
        }
    }

    uint8_t *src = (uint8_t *)data;
    uint8_t *dest = ((uint8_t *)array->array) + array->length * array->size;

    memcpy(dest, src, array->size);
    array->length++;

    return 0;
}

int array_pop_back(array_t *array)
{
    assert(array);

    if (array->length == 0) {
        return 0;
    }

    uint8_t *dest = ((uint8_t *)array->array) + array->length * array->size;

    memset(dest, 0, array->size);
    array->length--;

    if (array->length < array->capacity >> 1) {
        array = array_reduce(array);
        if (!array) {
            return -1;
        }
    }

    return 0;
}
