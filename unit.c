/*
 * Copyright (c) 2019 Tom Hancocks
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "unit.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#if defined(UNIT_TEST)

struct test_case {
	const char *name;
	int failed;
	test_case_func_t imp;
};

struct suite {
	const char *name;
	ssize_t count;
	ssize_t capacity;
	int failed;
	struct test_case **cases;
};

static ssize_t suite_count = 0;
static ssize_t suite_capacity = 0;
static struct suite **test_suites = NULL;

static struct suite *test_suite_named(const char *name)
{
    if (test_suites == NULL) {
        suite_capacity = 1;
        test_suites = calloc(suite_capacity, sizeof(*test_suites));
    }

    // Try and find the suite first, only add it if we can't find it.
    for (int i = 0; i < suite_count; ++i) {
        struct suite *s = test_suites[i];
        if (strcmp(s->name, name) == 0) {
            return s;
        }
    }

    // Do we have room for a new suite?
    if (suite_count > suite_capacity) {
        suite_capacity <<= 1; // Double the amount of space.
        test_suites = realloc(test_suites, sizeof(struct suite *) * suite_capacity);
    }

    // Add the suite, and then configure it.
    struct suite *s = calloc(1, sizeof(*s));
    test_suites[suite_count++] = s;

    s->name = name;
    s->capacity = 1;
    s->count = 0;
    s->failed = 0;
    s->cases = calloc(s->capacity, sizeof(*s->cases));

    return s;
}

static struct test_case *test_case_named(struct suite *s, const char *restrict name)
{
    // Do we have room a for a new test case?
    if (s->count > s->capacity) {
        s->capacity <<= 1; // Double the amount of space.
        s->cases = realloc(s->cases, sizeof(struct test_case *) * s->capacity);
    }

    // Add the suite and then configure it.
    struct test_case *tc = calloc(1, sizeof(*tc));
    s->cases[s->count++] = tc;

    tc->name = name;

    return tc;
}

void register_test(const char *restrict suite, const char *restrict name, test_case_func_t imp)
{
    struct suite *s = test_suite_named(suite);
    struct test_case *tc = test_case_named(s, name);
    tc->imp = imp;
}

int start_tests(void)
{
	ssize_t failed_test_count = 0;
	ssize_t test_count = 0;

    for (int i = 0; i < suite_count; ++i) {
        struct suite *s = test_suites[i];
        printf(
            "\x1b[31m❯\x1b[1;33m❯\x1b[0;32m❯ "
            "\x1b[36mStarting test suite \"\x1b[96m%s\x1b[36m\"\x1b[0m\n",
            s->name
        );

        // Give a summary about the test suite.
        printf(
            "\tTest suite contains %zu test case%s.\n",
            s->count, s->count == 1 ? "" : "s"
        );

        for (int j = 0; j < s->count; ++j) {
            struct test_case *tc = s->cases[j];
            tc->imp(s, tc);

            failed_test_count += tc->failed;
            test_count++;

            printf(
                "\t%s \x1b[0mTest case \"\x1b[1m%s\x1b[0m\" %s.\n",
                tc->failed == 0 ? "\x1b[0;32m✔" : "\x1b[0;31m×",
                tc->name,
                tc->failed == 0 ? "passed" : "failed"
            );
        }

        printf(
			"\t\x1b[1m%ld\x1b[0m tests run with "
			"\x1b[1m%ld\x1b[0m passed and \x1b[1m%d\x1b[0m failed.\n\n",
			s->count, 
			s->count - s->failed, 
			s->failed
		);
    }

    printf(
		"\x1b[1m%zu\x1b[0m tests run across \x1b[1m%zu\x1b[0m suites with "
		"\x1b[1m%zu\x1b[0m passed and \x1b[1m%zu\x1b[0m failed.\n\n",
		test_count, suite_count,
		test_count - failed_test_count, 
		failed_test_count
	);

    // Return 0 if there were no failed tests, otherwise return 1.
    return (failed_test_count > 1);
}


void pass_assertion(void *a, void *b, const char *condition)
{
	// Do nothing...
}

void fail_assertion(void *a, void *b, const char *condition)
{
	struct suite *s = a;
	struct test_case *tc = b;

	s->failed++;
	tc->failed++;
}

#if defined(INC_MAIN)

TEST_CASE(Alpha, Test1)
{
	ASSERT_EQ(0, 0);
}

TEST_CASE(Beta, First)
{
	ASSERT_EQ(0, 1);
}

TEST_CASE(Alpha, Test2)
{
	// ASSERT_EQ_STR("Hello", "Hello");
}

int main(int argc, const char **argv)
{
    return start_tests();
}

#endif
#endif
