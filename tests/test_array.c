#include <array.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ctest.h"
#include "unity.h"

TEST(test_array_init)
{
    array_t *array = array_init(10, sizeof(uint32_t));

    TEST_ASSERT_EQUAL(10, array->capacity);
    TEST_ASSERT_EQUAL(0, array->length);
    TEST_ASSERT_EQUAL(4, array->element_size);

    array_free(array);
}

TEST(test_array_set)
{
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
            TEST_ASSERT_EQUAL(test_val, result);
        } else {
            TEST_ASSERT_EQUAL(i, result);
        }
    }

    array_free(array);


}

/* TEST(test_array_set_out_of_bounds) */
/* { */
/*     bool fail = false; */
/*     (void) fail; */
/*     array_t *array = array_init(10, sizeof(size_t)); */

/*     for (size_t i = 0; i < array->capacity; i++) { */
/*         array_push_back(array, &i, sizeof(size_t)); */
/*     } */

/*     size_t test_val = 7357; */
/*     int result = 0; */

/*     result = array_set(array, 100000, &test_val, sizeof(size_t)); */
/*     fail = result != -1; */


/* } */

TEST(test_array_get)
{
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    for (size_t i = 0; i < 10; i++) {
        size_t result;
        array_get(array, i, &result, sizeof(size_t));
        TEST_ASSERT_EQUAL(i, result);
    }
}

/* TEST(test_array_get_out_of_bounds) */
/* { */
/*     bool fail = false; */
/*     (void) fail; */
/*     array_t *array = array_init(10, sizeof(size_t)); */

/*     for (size_t i = 0; i < 10; i++) { */
/*         array_push_back(array, &i, sizeof(size_t)); */
/*     } */

/*     size_t i; */
/*     int result = array_get(array, 10000, &i, sizeof(size_t)); */

/*     fail = result != -1; */


/* } */

TEST(test_array_get_reference)
{
    struct test {size_t val;};
    array_t *array = array_init(10, sizeof(struct test));

    for (size_t i = 0; i < 10; i++) {
        struct test t = {.val = i};
        array_push_back(array, &t, sizeof(struct test));
    }

    struct test *t = NULL;
    for (size_t i = 0; i < 10; i++) {
        array_get_reference(array, i, &t, sizeof(struct test));
        TEST_ASSERT_EQUAL(t->val, i);
        t->val = i+1;
    }

    for (size_t i = 0; i < 10; i++) {
        array_get_reference(array, i, &t, sizeof(struct test));
        TEST_ASSERT_EQUAL(t->val, i+1);
    }


}

TEST(test_array_insert)
{
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
            TEST_ASSERT_EQUAL(test_val, result);
        } else {
            TEST_ASSERT_EQUAL(0, result);
        }
    }

    array_free(array);
}

/* TEST(test_array_insert_out_of_bounds) */
/* { */
/*     bool fail = false; */
/*     (void) fail; */
/*     array_t *array = array_init(10, sizeof(size_t)); */

/*     for (size_t i = 0; i < 10; i++) { */
/*         array_push_back(array, &i, sizeof(size_t)); */
/*     } */

/*     size_t test_val = 7357; */
/*     fail = array_insert(array, 100, &test_val, sizeof(size_t)) != -1; */


/* } */

TEST(test_array_insert_expand)
{
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    TEST_ASSERT_EQUAL(10, array->capacity);

    size_t test_val = 7357;
    array_insert(array, 5, &test_val, sizeof(size_t));
    TEST_ASSERT_EQUAL(20, array->capacity);

    array_free(array);
}

TEST(test_array_remove)
{
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    TEST_ASSERT_EQUAL(10, array->length);

    array_remove(array, 0, sizeof(size_t)); // remove 0
    array_remove(array, 5, sizeof(size_t)); // remove 6
    array_remove(array, 7, sizeof(size_t)); // remove 9

    TEST_ASSERT_EQUAL(array->length, 7);

    size_t expected[] = {1, 2, 3, 4, 5, 7, 8};
    for (size_t i = 0; i < array->length; i++) {
        size_t val;
        array_get(array, i, &val, sizeof(size_t));
        TEST_ASSERT_EQUAL(expected[i], val);
    }

    array_free(array);
}

/* TEST(test_array_remove_out_of_bounds) */
/* { */
/*     bool fail = false; */
/*     (void) fail; */
/*     array_t *array = array_init(10, sizeof(size_t)); */

/*     for (size_t i = 0; i < 10; i++) { */
/*         array_push_back(array, &i, sizeof(size_t)); */
/*     } */

/*     fail = array_remove(array, 100, sizeof(size_t)) != -1; */

/*     array_free(array); */



/* } */

TEST(test_array_remove_reduce)
{
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    TEST_ASSERT_EQUAL(10, array->capacity);

    size_t test_val = 7357;
    array_push_back(array, &test_val, sizeof(size_t));
    TEST_ASSERT_EQUAL(20, array->capacity);

    array_remove(array, 5, sizeof(size_t));
    array_remove(array, 5, sizeof(size_t));

    TEST_ASSERT_EQUAL(10, array->capacity);

    array_free(array);
}

TEST(test_array_push_back)
{
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    for (size_t i = 0; i < 10; i++) {
        size_t val;
        TEST_ASSERT(array_get(array, i, &val, sizeof(size_t)) == 0);
        TEST_ASSERT_EQUAL(i, val);
    }

    array_free(array);
}

TEST(test_array_push_back_expand)
{
    array_t *array = array_init(10, sizeof(size_t));


    for (size_t i = 0; i < 11; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }


    array_free(array);

}

TEST(test_array_pop_back)
{
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    size_t val;

    TEST_ASSERT_EQUAL(10, array->length);
    array_pop_back(array, &val, sizeof(size_t));
    TEST_ASSERT_EQUAL(9, array->length);
    TEST_ASSERT_EQUAL(9, val);

    TEST_ASSERT(array_get(array, array->length-1, &val, sizeof(size_t)) == 0);
    TEST_ASSERT_EQUAL(8, val);

    array_free(array);

}

TEST(test_array_pop_back_reduce)
{
    array_t *array = array_init(10, sizeof(size_t));

    for (size_t i = 0; i < 10; i++) {
        array_push_back(array, &i, sizeof(size_t));
    }

    TEST_ASSERT_EQUAL(10, array->capacity);

    size_t test_val = 7357;
    size_t tmp;
    array_push_back(array, &test_val, sizeof(size_t));
    TEST_ASSERT_EQUAL(20, array->capacity);
    array_pop_back(array, &tmp, sizeof(size_t));
    array_pop_back(array, &tmp, sizeof(size_t));
    TEST_ASSERT_EQUAL(10, array->capacity);

    array_free(array);


}

/* int main(int argc, char **argv) */
/* { */
/*     bool all_tests_passed = true; */
/*     bool fail = false; */

/*     RUN_TEST(test_array_init); */
/*     RUN_TEST(test_array_set); */
/*     //RUN_TEST(test_array_set_out_of_bounds); */
/*     RUN_TEST(test_array_get); */
/*     //RUN_TEST(test_array_get_out_of_bounds); */
/*     RUN_TEST(test_array_get_reference); */
/*     RUN_TEST(test_array_insert); */
/*     //RUN_TEST(test_array_insert_out_of_bounds); */
/*     RUN_TEST(test_array_insert_expand); */
/*     RUN_TEST(test_array_remove); */
/*     //RUN_TEST(test_array_remove_out_of_bounds); */
/*     RUN_TEST(test_array_remove_reduce); */
/*     RUN_TEST(test_array_push_back); */
/*     RUN_TEST(test_array_push_back_expand); */
/*     RUN_TEST(test_array_pop_back); */
/*     RUN_TEST(test_array_pop_back_reduce); */

/*     if (all_tests_passed) { */
/*         printf("All test cases PASSED.\n"); */
/*     } else { */
/*         printf("Some test cases FAILED.\n"); */
/*     } */

/*     return 0; */
/* } */
