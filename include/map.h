#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

#include <array.h>
#include <stddef.h>

#define map_element_t(T) \
    struct {             \
        size_t key;      \
        T *value;        \
    }

#define map_t(key_T, value_T)                         \
    struct {                                          \
        size_t nbuckets;                              \
        size_t key_size;                              \
        size_t value_size;                            \
        size_t (*hash)(void *, size_t);               \
        bool (*key_is_equal)(void *, void *, size_t); \
        array_t(array_generic_t *) *buckets;          \
    }

typedef struct map_generic {
    size_t nbuckets;
    size_t key_size;
    size_t value_size;
    size_t (*hash)(void *, size_t);
    bool (*key_is_equal)(void *, void *, size_t);
    array_t(array_generic_t *) *buckets;
} map_generic_t;


#define map_init(key_T, value_T, nbuckets, hash_function, equal_function) (_map_init(nbuckets, sizeof(key_T), sizeof(value_T), hash_function, equal_function))
void *_map_init(size_t nbuckets, size_t key_size, size_t value_size,
                size_t (*hash)(void *, size_t), bool (*key_is_equal)(void *, void *, size_t));

void map_free(void *map);

#define map_set(key_T, value_T, map, key, value) \
    (_map_set((map_generic_t *)map, &(key_T){key}, sizeof(key_T), &(value_T){value}, sizeof(value_T)))
int _map_set(map_generic_t*map, void *key, size_t key_size, void *value, size_t value_size);

#define map_get(key_T, value_T, map, key) \
    (*(value_T *)_map_get((map_generic_t *)map, &(key_T){key}, sizeof(key_T), sizeof(value_T)))
void *_map_get(map_generic_t*map, void *key, size_t key_size, size_t value_size);

#define map_remove(key_T, value_T, map, key) \
    (_map_remove((map_generic_t *)map, &(key_T){key}, sizeof(key_T), sizeof(value_T)))
void _map_remove(map_generic_t*map, void *key, size_t key_size, size_t value_size);

#define map_key_exists(key_T, value_T, map, key) (_map_key_exists((map_generic_t *)map, &(key_T){key}, sizeof(key_T), sizeof(value_T)))
bool _map_key_exists(map_generic_t*map, void *key, size_t key_size, size_t value_size);

// Default key functions
size_t map_hash_uint(void *key, size_t key_size);
bool map_key_is_equal_uint(void *key1, void *key2, size_t key_size);

size_t map_hash_string(void *key, size_t key_size);
bool map_key_is_equal_string(void *key1, void *key2, size_t key_size);


#endif
