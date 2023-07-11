#include <map.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define RUN_TEST(test_func) do {                \
        fail = test_func();                     \
        printf(#test_func ": ");                \
        if (fail) {                             \
            printf("FAILED\n");                 \
            all_tests_passed = false;           \
        } else {                                \
            printf("PASSED\n");                 \
        }                                       \
} while(0)

bool test_map_init()
{
    bool fail = false;

    map_t *map = NULL;
    map = map_init(16, sizeof(size_t));
    if (!map) {
        return true;
    }

    fail |= map->nbuckets != 16;
    fail |= map->value_size != sizeof(size_t);
    fail |= map->key_array == NULL;
    fail |= map->map_array == NULL;

    map_free(map);
    return fail;
}

bool test_map_set()
{
    bool fail = false;

    map_t *map = NULL;
    map = map_init(16, sizeof(size_t));

    size_t test_val = 7357;
    size_t test_result;
    map_set(map, 0, &test_val, sizeof(size_t));
    map_get(map, 0, &test_result, sizeof(size_t));

    fail |= test_val != test_result;

    map_free(map);
    return fail;
}

bool test_map_get()
{
    bool fail = false;

    map_t *map = NULL;
    map = map_init(16, sizeof(size_t));

    for (size_t i = 0; i < 32; i++) {
        map_set(map, i, &i, sizeof(size_t));
    }

    for (size_t i = 0; i < 32; i++) {
        size_t v;
        map_get(map, i, &v, sizeof(size_t));
        fail |= v != i;
    }

    map_free(map);
    return fail;
}

bool test_map_get_reference()
{
    bool fail = false;
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
        fail |= t->val != i;
        t->val = i+1;
    }

    for (size_t i = 0; i < 32; i++) {
        map_get_reference(map, i, &t, sizeof(struct test));
        fail |= t->val != i+1;
    }

    map_free(map);
    return fail;
}

bool test_map_remove()
{
    bool fail = false;

    map_t *map = NULL;
    map = map_init(16, sizeof(size_t));

    for (size_t i = 0; i < 32; i++) {
        map_set(map, i, &i, sizeof(size_t));
    }

    map_remove(map, 4, sizeof(size_t));
    map_remove(map, 29, sizeof(size_t));
    map_remove(map, 10, sizeof(size_t));
    map_remove(map, 17, sizeof(size_t));

    fail |= map->key_array->length != 28;
    fail |= map_remove(map, 100, sizeof(size_t)) != -1;

    for (size_t i = 0; i < map->key_array->length; i++) {
        int result;
        size_t v;
        result = map_get(map, i, &v, sizeof(size_t));
        if (i == 4  || i == 29 || i == 10 || i == 17) {
            fail |= result != -1;

        } else {
            fail |= result != 0;
            fail |= v != i;
        }
    }

    map_free(map);
    return fail;
}

bool test_map_is_empty()
{
    bool fail = false;

    map_t *map_empty = map_init(16, sizeof(size_t));
    map_t *map_not_empty = map_init(16, sizeof(size_t));
    size_t test_val = 7357;
    map_set(map_not_empty, 0, &test_val, sizeof(size_t));

    fail |= map_is_empty(map_empty) != true;
    fail |= map_is_empty(map_not_empty) != false;

    map_free(map_empty);
    map_free(map_not_empty);
    return fail;
}

bool test_map_key_exists()
{
    bool fail = false;

    map_t *map1 = map_init(16, sizeof(size_t));
    map_t *map2 = map_init(16, sizeof(size_t));
    size_t test_val = 7357;

    map_set(map1, 0, &test_val, sizeof(size_t));
    map_set(map2, 1, &test_val, sizeof(size_t));

    fail |= map_key_exists(map1, 0) != true;
    fail |= map_key_exists(map2, 0) != false;

    map_free(map1);
    map_free(map2);
    return fail;
}



int main(int argc, char **argv)
{
    bool all_tests_passed = true;
    bool fail = false;

    RUN_TEST(test_map_init);
    RUN_TEST(test_map_set);
    RUN_TEST(test_map_get);
    RUN_TEST(test_map_get_reference);
    RUN_TEST(test_map_remove);
    RUN_TEST(test_map_is_empty);
    RUN_TEST(test_map_key_exists);


    if (all_tests_passed) {
        printf("All test cases PASSED.\n");
    } else {
        printf("Some test cases FAILED.\n");
    }

    return 0;
}
