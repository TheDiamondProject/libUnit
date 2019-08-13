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

#include <stdint.h>
#include <string.h>
#include <sys/types.h>

#if !defined(libUnit) && defined(UNIT_TEST)
#define libUnit

typedef void(*test_case_func_t)(void *, void *);

#define XSTR(_s) #_s
#define STR(_s) XSTR(_s)

#define ASSERT(_cond) \
	if (!(_cond)) { \
		fail_assertion(suite, test, STR(_cond)); \
		return; \
	} else { \
		pass_assertion(suite, test, STR(_cond)); \
	}

#define ASSERT_EQ(_a, _b) ASSERT((_a) == (_b))
#define ASSERT_EQ_STR(_a, _b) ASSERT(strcmp((_a), (_b)) == 0)

#define TEST_CASE(_suite, _name) \
	void __impl_##_suite##_##_name(void *, void *); \
	__attribute__((constructor)) void register_##_suite##_##_name(void) { \
		register_test(STR(_suite), STR(_name), __impl_##_suite##_##_name); \
	}\
	void __impl_##_suite##_##_name(void *suite, void *test)

void register_test(const char *restrict suite, const char *restrict name, test_case_func_t imp);
int start_tests(void);

void pass_assertion(void *, void *, const char *);
void fail_assertion(void *, void *, const char *);

#endif
