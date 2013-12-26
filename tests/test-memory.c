#include "../memory.h"
#include "minunit.h"

BEGIN_TESTS

initMemory();

BEGIN_SUITE("High and low functions",

	u8 number = 255;
	mu_assert("low4 of 255 is 15", getLowU4(number) == 15);
	mu_assert("high4 of 255 is 15", getHighU4(number) == 15);

	number = 195;
	mu_assert("low4 of 195 is 3", getLowU4(number) == 3);
	mu_assert("high4 of 195 is 12", getHighU4(number) == 12);
);


BEGIN_SUITE("Stack operations",

	u8 stack = 0;

	pushStack(&stack, 2503);
	pushStack(&stack, 100);
	pushStack(&stack, 5000);

	mu_assert("should be 5000", popStack(&stack) == 5000);
	mu_assert("should be 100", popStack(&stack) == 100);
	mu_assert("should be 2503", popStack(&stack) == 2503);
);


BEGIN_SUITE("Convert operations",

	u8 number[] = STATIC_INIT({ 0xaa, 0xbb, 0xcc, 0xdd });

	mu_assert("u32 should be 0xaabbccdd", convertBytesToU32(number, 4) == 0xAABBCCDD );
	mu_assert("u12 should be 0xbbccdd", convertBytesToU12(number) == 0xABB );
);


BEGIN_SUITE("Convert operations",

	storeToMemory(100, 33);
	storeToMemory(101, 255);
	storeToMemory(102, 111);

	mu_assert("should be 33", getFromMemory(100) == 33 );
	mu_assert("should be 255", getFromMemory(101) == 255 );
	mu_assert("should be 111", getFromMemory(102) == 111 );
);


END_TESTS_AUTO_RUN

