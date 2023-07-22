#include <map.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ctest.h"
#include "unity.h"

TEST(test_map_init)
{
    map_t(size_t, size_t) *map = map_init(size_t, size_t, 16, map_hash_uint, map_key_is_equal_uint);
    TEST_ASSERT(map);


    TEST_ASSERT_EQUAL(16, map->nbuckets);
    TEST_ASSERT_EQUAL(sizeof(size_t), map->value_size);
    TEST_ASSERT(map->buckets);

    map_free(map);
}

TEST(test_map_set)
{
    map_t(size_t, size_t) *map = map_init(size_t, size_t, 16, map_hash_uint, map_key_is_equal_uint);

    size_t test_val = 7357;
    size_t test_result;
    map_set(size_t, size_t, map, 0, test_val);
    test_result = map_get(size_t, size_t, map, 0);

    TEST_ASSERT_EQUAL(test_val, test_result);

    map_free(map);

}

TEST(test_map_get)
{
    map_t(size_t, size_t) *map = map_init(size_t, size_t, 16, map_hash_uint, map_key_is_equal_uint);

    for (size_t i = 0; i < 32; i++) {
        map_set(size_t, size_t, map, i, i);
    }

    for (size_t i = 0; i < 32; i++) {
        size_t v;
        v = map_get(size_t, size_t, map, i);
        TEST_ASSERT_EQUAL(i, v);
    }

    map_free(map);

}

TEST(test_map_remove)
{
    map_t(size_t, size_t) *map = map_init(size_t, size_t, 16, map_hash_uint, map_key_is_equal_uint);

    for (size_t i = 0; i < 32; i++) {
        map_set(size_t, size_t, map, i, i);
    }

    map_remove(size_t, size_t, map, 4);
    map_remove(size_t, size_t, map, 29);
    map_remove(size_t, size_t, map, 10);
    map_remove(size_t, size_t, map, 17);

    for (size_t i = 0; i < 32; i++) {
        bool key_exists = map_key_exists(size_t, size_t, map, i);
        if (i == 4  || i == 29 || i == 10 || i == 17) {
            TEST_ASSERT_EQUAL(false, key_exists);
        } else {
            TEST_ASSERT_EQUAL(true, key_exists);
        }
    }

    map_free(map);

}
