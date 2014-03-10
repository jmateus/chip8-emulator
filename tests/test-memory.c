#include "../src/memory.h"
#include "../src/graphics.h"

#include "minunit.h"

BEGIN_TESTS("Memory")

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


BEGIN_SUITE("Get memory",

	storeToMemory(200, 0xAB);
	storeToMemory(250, 0xCD);

	mu_assert("should return correct memory pointer", *(getMemory(200)) == 0xAB );
	mu_assert("should return correct memory pointer", *(getMemory(250)) == 0xCD );

);


BEGIN_SUITE("Load data",

	u8 data[] = STATIC_INIT({ 0xaa, 0xbb, 0xcc, 0xdd });

	loadData(data, 4, 100);

	int i;
	for(i = 0; i < 4; i++) {
		mu_assert("should load a array of data", getFromMemory(100 + i) == data[i]);
	}
	
);


BEGIN_SUITE("Load program",

	loadProgram("test files/maze.ch8");

	mu_assert("should load a program correctly", getFromMemory(PROGRAM_OFFSET) == 0x60);
	mu_assert("should load a program correctly", getFromMemory(PROGRAM_OFFSET + 16) == 0x30);
	mu_assert("should load a program correctly", getFromMemory(PROGRAM_OFFSET + 37) == 0x10);

);


BEGIN_SUITE("Load charset",

	loadDefaultCharset((u8*) CHIP8_DEFAULT_CHARSET, DEFAULT_CHARSET_SIZE);

	mu_assert("should load a charset correctly", 
		getFromMemory(CHARSET_OFFSET) == (CHIP8_DEFAULT_CHARSET[0][0]));
	mu_assert("should load a charset correctly", 
		getFromMemory(CHARSET_OFFSET + (NUMBER_CHARS-1) * DEFAULT_CHARS_SIZE + 2) 
		== (CHIP8_DEFAULT_CHARSET[NUMBER_CHARS-1][2]));
	mu_assert("should load a charset correctly", 
		getFromMemory(CHARSET_OFFSET + (NUMBER_CHARS-1) * DEFAULT_CHARS_SIZE + DEFAULT_CHARS_SIZE - 1) 
		== (CHIP8_DEFAULT_CHARSET[NUMBER_CHARS-1][DEFAULT_CHARS_SIZE - 1]));

);


BEGIN_SUITE("Return char location",

	mu_assert("should return the correct location of a char", getCharLocation(0x9) 
		== (CHARSET_OFFSET + DEFAULT_CHARS_SIZE*0x9) );
	mu_assert("should return the correct location of a char", getCharLocation(0xf) 
		== (CHARSET_OFFSET + DEFAULT_CHARS_SIZE*0xf) );

);



END_TESTS_AUTO_RUN

