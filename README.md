# libUnit ![BuildCIBadge](https://travis-ci.org/TheDiamondProject/libUnit.svg?branch=master)
**libUnit** is part of _The Diamond Project_ and is a simple custom unit testing
mechanism for all C components and libraries of the project.

## Usage
To use **libUnit** you simply need to include `unit.c` and `unit.h` files into
your project. This may be done by either manually adding those files to your
project or by using a `git submodule`.

Here is an example of how to use **libUnit**.

```c
// main.c
#include "unit.h"
#include <stdio.h>

int sum(int a, int b)
{
	return a + b;
}

TEST_CASE(SumFunction, ExpectedResult)
{
	int result = sum(13, 12);
	ASSERT_EQ(result, 25);
}

int main(int argc, const char **argv)
{
#ifdef UNIT_TEST
	return start_tests();
#else
	return sum(5, 4);
#endif
}
```

And to build this your compiler invocation would look something like:

```sh
cc -DUNIT_TEST -o main-test main.c unit.c
```

## Testing
To test **libUnit** you will need to have a C Compiler on your machine,
such as `gcc` or `clang`. You will also need to have `make` installed.

To build and run the tests for **libUnit** you simply need to enter the 
following command:

```sh
make run-all-tests 
```

This will build an executable that will test the various aspects of **libUnit**
in order to confirm that it all works as expected.

## License
**libUnit** is provided by the Diamond Project under the MIT License.
 
