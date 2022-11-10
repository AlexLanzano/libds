#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <array.h>
#include <map.h>

#include <stdio.h>
static size_t map_hash(map_t *map, size_t val)
{
    assert(map);
    return val % map->nbuckets;
}

static array_t *map_get_bucket_from_key(map_t *map, size_t key)
{
    size_t hash = map_hash(map, key);
    array_t *bucket = NULL;
    array_get(map->map_array, hash, &bucket);
    return bucket;
}

static int map_find_value(map_t *map, size_t key, array_t *bucket, void *value)
{
    assert(map);
    assert(bucket);

    bool key_found = false;
    uint8_t data[sizeof(size_t)+map->element_size];

    for (size_t i = 0; i < bucket->length; i++) {
        array_get(bucket, i, data);

        uint64_t data_key = *(uint64_t *)data;
        if (key == data_key) {
            key_found = true;
            break;
        }
    }

    if (key_found == false) {
        return -1;
    }

    uint8_t *value_ptr = data + sizeof(size_t);
    memcpy(value, value_ptr, map->element_size);

    return 0;
}

static int map_find_index(map_t *map, size_t key, array_t *bucket, size_t *index)
{
    assert(map);
    assert(bucket);
    assert(index);

    uint8_t data[sizeof(size_t)+map->element_size];

    for (size_t i = 0; i < bucket->length; i++) {
        array_get(bucket, i, data);

        uint64_t data_key = *(uint64_t *)data;
        if (key == data_key) {
            *index = i;
            return 0;
        }
    }

    return -1;
}

static int map_find_index_key_array(map_t *map, size_t key, array_t *key_array, size_t *index)
{
    assert(map);
    assert(key_array);
    assert(index);

    size_t data_key;
    for (size_t i = 0; i < key_array->length; i++) {
        array_get(key_array, i, &data_key);
        if (key == data_key) {
            *index = i;
            return 0;
        }
    }

    return -1;
}

map_t *map_init(size_t nbuckets, size_t element_size)
{
    if (nbuckets == 0 || element_size == 0) {
        return NULL;
    }

    map_t *map = calloc(1, sizeof(map_t));
    if (!map) {
        return NULL;
    }

    map->nbuckets = nbuckets;
    map->element_size = element_size;

    map->key_array = array_init(16, sizeof(size_t));
    if (!map->key_array) {
        free(map);
        return NULL;
    }

    map->map_array = array_init(nbuckets, sizeof(array_t *));
    if (!map->map_array) {
        free(map->key_array);
        free(map);
        return NULL;
    }

    for (size_t i = 0; i< nbuckets; i++) {
        array_t *bucket = array_init(16, sizeof(size_t) + element_size);
        if (!bucket) {
            free(map->map_array);
            free(map->key_array);
            free(map);
            return NULL;
        }
        array_push_back(map->map_array, &bucket);
    }

    return map;
}

void map_free(map_t *map)
{
    if (!map) {
        return;
    }

    for (size_t i = 0; i < map->nbuckets; i++) {
        array_t *bucket;
        array_get(map->map_array, i, &bucket);
        array_free(bucket);
    }
    array_free(map->map_array);
    array_free(map->key_array);
    free(map);
}

int map_set(map_t *map, size_t key, void *value)
{
    if (!map || !value) {
        return -1;
    }

    array_push_back(map->key_array, &key);

    array_t *bucket = map_get_bucket_from_key(map, key);
    uint8_t data[sizeof(size_t)+map->element_size];
    uint8_t *value_ptr = data + sizeof(size_t);

    memcpy(data, &key, sizeof(size_t));
    memcpy(value_ptr, value, map->element_size);

    array_push_back(bucket, data);

    return 0;
}

int map_get(map_t *map, size_t key, void *value)
{
    if (!map || !value) {
        return -1;
    }

    array_t *bucket = map_get_bucket_from_key(map, key);
    return map_find_value(map, key, bucket, value);
}
int map_remove(map_t *map, size_t key)
{
    if (!map) {
        return -1;
    }

    size_t index;
    array_t *bucket = map_get_bucket_from_key(map, key);
    if (map_find_index(map, key, bucket, &index) == -1) {
        return -1;
    }
    array_remove(bucket, index);

    if (map_find_index_key_array(map, key, map->key_array, &index) == -1) {
        return -1;
    }
    array_remove(map->key_array, index);

    return 0;
}

bool map_is_empty(map_t *map)
{
    if (!map) {
        return -1;
    }

    return (map->key_array->length == 0) ? true : false;
}

bool map_key_exists(map_t *map, size_t key)
{
    if (!map) {
        return -1;
    }

    size_t index;
    return (map_find_index_key_array(map, key, map->key_array, &index) == -1) ? false : true;
}
