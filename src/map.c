#include "unity_internals.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <array.h>
#include <map.h>

#include <stdio.h>

static array_generic_t *map_get_bucket(map_generic_t *map, void *key)
{
    size_t key_hash = map->hash(key, map->key_size);
    size_t key_bucket = key_hash % map->nbuckets;
    return ((array_generic_t **)array(map->buckets))[key_bucket];
}

static void *map_find_value_ref_from_bucket(map_generic_t *map, array_generic_t *bucket, void *key)
{
    array_for_each(void *, element, bucket) {
        void *element_key = element;
        if (map->key_is_equal(element_key, key, map->key_size)) {
            return (uint8_t *)element + map->key_size;
        }
    }

    return NULL;
}

static void array_remove_from_bucket(map_generic_t *map,array_generic_t *bucket, void *key)
{
    for (size_t i = 0; i < bucket->length; i++) {
        void *element_key = (uint8_t *)array(bucket) + (i * bucket->element_size);
        if (map->key_is_equal(element_key, key, map->key_size)) {
            array_remove(bucket, i);
            break;
        }
    }
}

void *_map_init(size_t nbuckets, size_t key_size, size_t value_size,
                size_t (*hash)(void *, size_t), bool (*key_is_equal)(void *, void *, size_t))
{
    assert(nbuckets > 0);
    assert(key_size > 0);
    assert(value_size > 0);
    assert(hash != NULL);

    map_generic_t *map = calloc(1, sizeof(map_generic_t));
    if (!map) {
        return NULL;
    }

    map->nbuckets = nbuckets;
    map->key_size = key_size;
    map->value_size = value_size;
    map->hash = hash;
    map->key_is_equal = key_is_equal;

    // Allocate generic array to contain the key (size_t) and the value (value_size)
    map->buckets = array_init_prealloc(array_generic_t *, nbuckets);
    if (!map->buckets) {
        free(map);
        return NULL;
    }

    for (size_t i = 0; i < nbuckets; ++i) {
        array(map->buckets)[i] = _array_init(16, key_size + value_size);
        if (array(map->buckets)[i] == NULL) {

            // Free all allocated buckets
            for (size_t i = 0; i < map->buckets->length; ++i) {
                array_free(array(map->buckets)[i]);
            }

            array_free(map->buckets);
            free(map);
            return NULL;
        }
    }

    return map;
}

void map_free(void *map)
{
    map_generic_t *generic_map = map;

    for (size_t i = 0; i < generic_map->buckets->length; ++i) {
        array_free(array(generic_map->buckets)[i]);
    }

    array_free(generic_map->buckets);

    free(generic_map);
}

int _map_set(map_generic_t *map, void *key, size_t key_size, void *value, size_t value_size)
{
    assert(map != NULL);
    assert(key != NULL);
    assert(key_size > 0);
    assert(value != NULL);
    assert(value_size > 0);

    assert(map->nbuckets > 0);
    assert(map->key_size == key_size);
    assert(map->value_size == value_size);
    assert(map->hash != NULL);
    assert(map->buckets != NULL);

    array_generic_t *bucket = map_get_bucket(map, key);
    void *value_ref;
    if ((value_ref = map_find_value_ref_from_bucket(map, bucket, key)) != NULL) {
        // Update existing map entry
        memcpy(value_ref, value, value_size);

    } else {
        // Create new map entry
        uint8_t element[key_size + value_size];
        void *key_ref = element;
        uint8_t *value_ref = element + key_size;
        memcpy(key_ref, key, key_size);
        memcpy(value_ref, value, value_size);
        _array_append(bucket, &element, key_size + value_size);
    }

    return 0;
}

void *_map_get(map_generic_t *map, void *key, size_t key_size, size_t value_size)
{
    assert(map != NULL);
    assert(key != NULL);
    assert(key_size > 0);
    assert(value_size > 0);

    assert(map->nbuckets > 0);
    assert(map->key_size == key_size);
    assert(map->value_size == value_size);
    assert(map->hash != NULL);
    assert(map->buckets != NULL);

    array_generic_t *bucket = map_get_bucket(map, key);

    return map_find_value_ref_from_bucket(map, bucket, key);
}

void _map_remove(map_generic_t *map, void *key, size_t key_size, size_t value_size)
{
    assert(map != NULL);
    assert(key != NULL);
    assert(key_size > 0);
    assert(value_size > 0);

    assert(map->nbuckets > 0);
    assert(map->key_size == key_size);
    assert(map->value_size == value_size);
    assert(map->hash != NULL);
    assert(map->buckets != NULL);

    array_generic_t *bucket = map_get_bucket(map, key);
    array_remove_from_bucket(map, bucket, key);
}

bool _map_key_exists(map_generic_t *map, void *key, size_t key_size, size_t value_size)
{
    assert(map != NULL);
    assert(key != NULL);
    assert(key_size > 0);
    assert(value_size > 0);

    assert(map->nbuckets > 0);
    assert(map->key_size == key_size);
    assert(map->value_size == value_size);
    assert(map->hash != NULL);
    assert(map->buckets != NULL);

    array_generic_t *bucket = map_get_bucket(map, key);

    return map_find_value_ref_from_bucket(map, bucket, key) != NULL ? true : false;
}

size_t map_hash_uint(void *key, size_t key_size)
{
    assert(key_size == sizeof(size_t) ||
           key_size == sizeof(uint64_t) ||
           key_size == sizeof(uint32_t) ||
           key_size == sizeof(uint16_t) ||
           key_size == sizeof(uint8_t));

    size_t hash = 0;
    memcpy(&hash, key, key_size);
    return hash;
}

bool map_key_is_equal_uint(void *key1, void *key2, size_t key_size)
{
    assert(key_size == sizeof(size_t) ||
           key_size == sizeof(uint64_t) ||
           key_size == sizeof(uint32_t) ||
           key_size == sizeof(uint16_t) ||
           key_size == sizeof(uint8_t));

    size_t key1_hash = 0;
    memcpy(&key1_hash, key1, key_size);

    size_t key2_hash = 0;
    memcpy(&key2_hash, key2, key_size);

    return key1_hash == key2_hash;
}

size_t map_hash_string(void *key, size_t size)
{
    assert(key != NULL);
    assert(size == sizeof(const char *));

    char * str = key;
    size_t hash = 5381;
    char c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

bool map_key_is_equal_string(void *key1, void *key2, size_t key_size)
{
    assert(key1 != NULL);
    assert(key2 != NULL);
    assert(key_size == sizeof(const char *));

    return strcmp(key1, key2) == 0 ? true : false;
}
