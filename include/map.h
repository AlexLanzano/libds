#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

#include <array.h>

typedef struct map {
    size_t nbuckets;
    size_t element_size;
    array_t *key_array;
    array_t *map_array;
} map_t;

map_t *map_init(size_t nbuckets, size_t element_size);
void map_free(map_t *map);
int map_set(map_t *map, size_t key, void *value);
int map_get(map_t *map, size_t key, void *value);
int map_get_reference(map_t *map, size_t key, void *reference);
int map_remove(map_t *map, size_t key);
bool map_is_empty(map_t *map);
bool map_key_exists(map_t *map, size_t key);

#endif
