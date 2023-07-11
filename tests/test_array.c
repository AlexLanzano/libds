#include <array.h>
#include <stddef.h>
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

bool test_array_init()
{
    bool fail = false;
    array_t *array = array_init(10, sizeof(uint32_t));

    fail |= array->capacity != 10;
    fail |= array->length != 0;
    fail |= array->element_size != 4;

    array_free(array);

    return fail;
}

bool test_array_set()
{
    bool fail = false;
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < array->capacity; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    size_t test_val = 7357;
    array_set(array, 0, &test_val, sizeof(size_t));
    array_set(array, 4, &test_val, sizeof(size_t));
    array_set(array, 9, &test_val, sizeof(size_t));

    for (size_t i = 0; i < array->capacity; i++) {
        size_t result = 99999;
        array_get(array, i, &result, sizeof(size_t));
        if (i == 0 || i == 4 || i == 9) {
            fail |= result != 7357;
        } else {
            fail |= result != i;
        }
    }

    array_free(array);

    return fail;
}

bool test_array_set_out_of_bounds()
{
    bool fail = false;
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < array->capacity; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    size_t test_val = 7357;
    int result = 0;

    result = array_set(array, 100000, &test_val, sizeof(size_t));
    fail = result != -1;

    return fail;
}

bool test_array_get()
{
    bool fail = false;
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    for (size_t i = 0; i < 10; i++) {
        size_t result;
        array_get(array, i, &result, sizeof(size_t));
        fail |= result != i;
    }

    return fail;
}

bool test_array_get_out_of_bounds()
{
    bool fail = false;
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    size_t i;
    int result = array_get(array, 10000, &i, sizeof(size_t));

    fail = result != -1;

    return fail;
}

bool test_array_get_reference()
{
    bool fail = false;
    struct test {size_t val;};
    array_t *array = array_init(10, sizeof(struct test));

    for (size_t i = 0; i < 10; i++) {
        struct test t = {.val = i};
        array_push_back(array, &t, sizeof(struct test));
    }

    struct test *t = NULL;
    for (size_t i = 0; i < 10; i++) {
        array_get_reference(array, i, &t, sizeof(struct test));
        fail |= t->val != i;
        t->val = i+1;
    }

    for (size_t i = 0; i < 10; i++) {
        array_get_reference(array, i, &t, sizeof(struct test));
        fail |= t->val != i+1;
    }

    return fail;
}

bool test_array_insert()
{
    bool fail = false;
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        size_t v = 0;
        array_push_back(array, &v, sizeof(size_t));
    }

    size_t v;
    array_get(array, 0, &v, sizeof(size_t));

    size_t test_val = 7357;
    array_insert(array, 0, &test_val, sizeof(size_t));
    array_insert(array, 5, &test_val, sizeof(size_t));
    array_insert(array, 10, &test_val, sizeof(size_t));

    for (size_t i = 0; i < array->length; i++) {
        size_t result;
        array_get(array, i, &result, sizeof(size_t));

        if (i == 0 || i == 5 || i == 10) {
            fail |= result != 7357;
        } else {
            fail |= result != 0;
        }
    }

    array_free(array);

    return fail;
}

bool test_array_insert_out_of_bounds()
{
    bool fail = false;
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    size_t test_val = 7357;
    fail = array_insert(array, 100, &test_val, sizeof(size_t)) != -1;

    return fail;
}

bool test_array_insert_expand()
{
    bool fail = false;
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    fail |= array->capacity != 10;

    size_t test_val = 7357;
    array_insert(array, 5, &test_val, sizeof(size_t));
    fail |= array->capacity != 20;

    array_free(array);

    return fail;
}

bool test_array_remove()
{
    bool fail = false;
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    fail |= array->length != 10;

    array_remove(array, 0, sizeof(size_t)); // remove 0
    array_remove(array, 5, sizeof(size_t)); // remove 6
    array_remove(array, 7, sizeof(size_t)); // remove 9

    fail |= array->length != 7;

    size_t expected[] = {1, 2, 3, 4, 5, 7, 8};
    for (size_t i = 0; i < array->length; i++) {
        size_t val;
        array_get(array, i, &val, sizeof(size_t));
        fail |= val != expected[i];
    }

    array_free(array);

    return fail;
}

bool test_array_remove_out_of_bounds()
{
    bool fail = false;
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    fail = array_remove(array, 100, sizeof(size_t)) != -1;

    array_free(array);

    return fail;

}

bool test_array_remove_reduce()
{
    bool fail = false;
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    fail |= array->capacity != 10;

    size_t test_val = 7357;
    array_push_back(array, &test_val, sizeof(size_t));
    fail |= array->capacity != 20;

    array_remove(array, 5, sizeof(size_t));
    array_remove(array, 5, sizeof(size_t));
    fail |= array->capacity != 10;

    array_free(array);

    return fail;
}

bool test_array_push_back()
{
    bool fail = false;
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    for (size_t i = 0; i < 10; i++) {
        size_t val;
        fail |= array_get(array, i, &val, sizeof(size_t));
        fail |= val != i;
    }

    array_free(array);
    return fail;
}

bool test_array_push_back_expand()
{
    bool fail = false;
    array_t *array = array_init(10, sizeof(size_t));

    fail |= array->capacity != 10;

    for (size_t i = 0; i < 11; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    fail |= array->capacity != 20;

    array_free(array);
    return fail;
}

bool test_array_pop_back()
{
    bool fail = false;
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    size_t val;

    fail |= array->length != 10;
    array_pop_back(array, &val, sizeof(size_t));
    fail |= array->length != 9;
    fail |= val != 9;

    fail |= array_get(array, array->length-1, &val, sizeof(size_t));
    fail |= val != 8;

    array_free(array);
    return fail;
}

bool test_array_pop_back_reduce()
{
    bool fail = false;
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    fail |= array->capacity != 10;

    size_t test_val = 7357;
    size_t tmp;
    array_push_back(array, &test_val, sizeof(size_t));
    fail |= array->capacity != 20;
    array_pop_back(array, &tmp, sizeof(size_t));
    array_pop_back(array, &tmp, sizeof(size_t));
    fail |= array->capacity != 10;

    array_free(array);

    return fail;
}

int main(int argc, char **argv)
{
    bool all_tests_passed = true;
    bool fail = false;

    RUN_TEST(test_array_init);
    RUN_TEST(test_array_set);
    //RUN_TEST(test_array_set_out_of_bounds);
    RUN_TEST(test_array_get);
    //RUN_TEST(test_array_get_out_of_bounds);
    RUN_TEST(test_array_get_reference);
    RUN_TEST(test_array_insert);
    //RUN_TEST(test_array_insert_out_of_bounds);
    RUN_TEST(test_array_insert_expand);
    RUN_TEST(test_array_remove);
    //RUN_TEST(test_array_remove_out_of_bounds);
    RUN_TEST(test_array_remove_reduce);
    RUN_TEST(test_array_push_back);
    RUN_TEST(test_array_push_back_expand);
    RUN_TEST(test_array_pop_back);
    RUN_TEST(test_array_pop_back_reduce);

    if (all_tests_passed) {
        printf("All test cases PASSED.\n");
    } else {
        printf("Some test cases FAILED.\n");
    }

    return 0;
}
