#include <array.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ctest.h"
#include "unity.h"

TEST(test_array_init)
{
    array_t(uint32_t) *array = array_init(uint32_t);

    TEST_ASSERT_EQUAL(0, array->capacity);
    TEST_ASSERT_EQUAL(0, array->length);
    TEST_ASSERT_EQUAL(4, array->element_size);

    array_free(array);
}

TEST(test_array_init_prealloc)
{
    array_t(uint32_t) *array = array_init_prealloc(uint32_t, 20);

    TEST_ASSERT_EQUAL(20, array->capacity);
    TEST_ASSERT_EQUAL(0, array->length);
    TEST_ASSERT_EQUAL(4, array->element_size);

    array_free(array);
}

TEST(test_array_set)
{
    array_t(size_t) *array = array_init(size_t);

    for (size_t i = 0; i < array->capacity; i++) {
        array_append(size_t, array, i);
    }

    size_t test_val = 7357;
    array(array)[0] = test_val;
    array(array)[4] = test_val;
    array(array)[9] = test_val;

    for (size_t i = 0; i < array->capacity; i++) {
        size_t result = array(array)[i];
        if (i == 0 || i == 4 || i == 9) {
            TEST_ASSERT_EQUAL(test_val, result);
        } else {
            TEST_ASSERT_EQUAL(i, result);
        }
    }

    array_free(array);


}

TEST(test_array_get)
{
    array_t(size_t) *array = array_init(size_t);

    for (size_t i = 0; i < 10; i++) {
        array_append(size_t, array, i);
    }

    for (size_t i = 0; i < 10; i++) {
        size_t result =  array(array)[i];
        TEST_ASSERT_EQUAL(i, result);
    }
}

TEST(test_array_insert)
{
    array_t(size_t) *array = array_init(size_t);

    for (size_t i = 0; i < 10; i++) {
        array_append(size_t, array, 0);
    }

    size_t test_val = 7357;
    array_insert(size_t, array, 0, test_val);
    array_insert(size_t, array, 5, test_val);
    array_insert(size_t, array, 10, test_val);

    for (size_t i = 0; i < array->length; i++) {
        size_t result = array->array[i];

        if (i == 0 || i == 5 || i == 10) {
            TEST_ASSERT_EQUAL(test_val, result);
        } else {
            TEST_ASSERT_EQUAL(0, result);
        }
    }

    array_free(array);
}

TEST(test_array_insert_expand)
{
    array_t(size_t) *array = array_init(size_t);

    TEST_ASSERT_EQUAL(0, array->capacity);

    for (size_t i = 0; i < 16; i++) {
        array_append(size_t, array, i);
    }

    TEST_ASSERT_EQUAL(16, array->capacity);

    array_insert(size_t, array, 5, 7357);
    TEST_ASSERT_EQUAL(32, array->capacity);

    array_free(array);
}

TEST(test_array_remove)
{
    array_t(size_t) *array = array_init(size_t);

    for (size_t i = 0; i < 10; i++) {
        array_append(size_t, array, i);
    }

    TEST_ASSERT_EQUAL(10, array->length);

    array_remove(array, 0);
    array_remove(array, 5);
    array_remove(array, 7);

    TEST_ASSERT_EQUAL(array->length, 7);

    size_t expected[] = {1, 2, 3, 4, 5, 7, 8};
    for (size_t i = 0; i < array->length; i++) {
        size_t val = array->array[i];
        TEST_ASSERT_EQUAL(expected[i], val);
    }

    array_free(array);
}

TEST(test_array_remove_reduce)
{
    array_t(size_t) *array = array_init(size_t);

    for (size_t i = 0; i < 16; i++) {
        array_append(size_t, array, i);
    }

    TEST_ASSERT_EQUAL(16, array->capacity);

    for (size_t i = 0; i < 9; i++) {
        array_remove(array, 5);
    }

    TEST_ASSERT_EQUAL(8, array->capacity);

    array_free(array);
}

TEST(test_array_append)
{
    array_t(size_t) *array = array_init(size_t);

    for (size_t i = 0; i < 10; i++) {
        array_append(size_t, array, i);
    }

    for (size_t i = 0; i < 10; i++) {
        TEST_ASSERT_EQUAL(i, array->array[i]);
    }

    array_free(array);
}

TEST(test_array_append_expand)
{
    array_t(size_t) *array = array_init(size_t);

    TEST_ASSERT_EQUAL(0, array->capacity);

    for (size_t i = 0; i < 16; i++) {
        array_append(size_t, array, i);
    }

    TEST_ASSERT_EQUAL(16, array->capacity);

    array_append(size_t, array, 10);

    TEST_ASSERT_EQUAL(32, array->capacity);

    array_free(array);
}
