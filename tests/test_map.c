#include <map.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ctest.h"
#include "unity.h"

TEST(test_map_init)
{
    map_t *map = NULL;
    map = map_init(16, sizeof(size_t));
    TEST_ASSERT(map);


    TEST_ASSERT_EQUAL(16, map->nbuckets);
    TEST_ASSERT_EQUAL(sizeof(size_t), map->value_size);
    TEST_ASSERT(map->key_array);
    TEST_ASSERT(map->map_array);

    map_free(map);
}

TEST(test_map_set)
{
    map_t *map = NULL;
    map = map_init(16, sizeof(size_t));

    size_t test_val = 7357;
    size_t test_result;
    map_set(map, 0, &test_val, sizeof(size_t));
    map_get(map, 0, &test_result, sizeof(size_t));

    TEST_ASSERT_EQUAL(test_val, test_result);

    map_free(map);

}

TEST(test_map_get)
{
    map_t *map = NULL;
    map = map_init(16, sizeof(size_t));

    for (size_t i = 0; i < 32; i++) {
        map_set(map, i, &i, sizeof(size_t));
    }

    for (size_t i = 0; i < 32; i++) {
        size_t v;
        map_get(map, i, &v, sizeof(size_t));
        TEST_ASSERT_EQUAL(i, v);
    }

    map_free(map);

}

TEST(test_map_get_reference)
{
    struct test {size_t val;};

    map_t *map = NULL;
    map = map_init(16, sizeof(size_t));

    for (size_t i = 0; i < 32; i++) {
        struct test t = {.val = i};
        map_set(map, i, &t, sizeof(struct test));
    }

    struct test *t = NULL;
    for (size_t i = 0; i < 32; i++) {
        map_get_reference(map, i, &t, sizeof(struct test));
        TEST_ASSERT_EQUAL(i, t->val);
        t->val = i+1;
    }

    for (size_t i = 0; i < 32; i++) {
        map_get_reference(map, i, &t, sizeof(struct test));
        TEST_ASSERT_EQUAL(i+1, t->val);
    }

    map_free(map);

}

TEST(test_map_remove)
{
    map_t *map = NULL;
    map = map_init(16, sizeof(size_t));

    for (size_t i = 0; i < 32; i++) {
        map_set(map, i, &i, sizeof(size_t));
    }

    map_remove(map, 4, sizeof(size_t));
    map_remove(map, 29, sizeof(size_t));
    map_remove(map, 10, sizeof(size_t));
    map_remove(map, 17, sizeof(size_t));

    TEST_ASSERT_EQUAL(28, map->key_array->length);
    TEST_ASSERT(map_remove(map, 100, sizeof(size_t)) == -1);

    for (size_t i = 0; i < map->key_array->length; i++) {
        int result;
        size_t v;
        result = map_get(map, i, &v, sizeof(size_t));
        if (i == 4  || i == 29 || i == 10 || i == 17) {
            TEST_ASSERT_EQUAL(-1, result);

        } else {
            TEST_ASSERT_EQUAL(0, result);
            TEST_ASSERT_EQUAL(i, v);
        }
    }

    map_free(map);

}

TEST(test_map_is_empty)
{
    map_t *map_empty = map_init(16, sizeof(size_t));
    map_t *map_not_empty = map_init(16, sizeof(size_t));
    size_t test_val = 7357;
    map_set(map_not_empty, 0, &test_val, sizeof(size_t));

    TEST_ASSERT(map_is_empty(map_empty) == true);
    TEST_ASSERT(map_is_empty(map_not_empty) == false);

    map_free(map_empty);
    map_free(map_not_empty);

}

TEST(test_map_key_exists)
{
    map_t *map1 = map_init(16, sizeof(size_t));
    map_t *map2 = map_init(16, sizeof(size_t));
    size_t test_val = 7357;

    map_set(map1, 0, &test_val, sizeof(size_t));
    map_set(map2, 1, &test_val, sizeof(size_t));

    TEST_ASSERT(map_key_exists(map1, 0) == true);
    TEST_ASSERT(map_key_exists(map2, 0) == false);

    map_free(map1);
    map_free(map2);

}
