#include "../cpu.h"
#include "minunit.h"

BEGIN_TESTS

initCPU();
initMemory();

BEGIN_SUITE("Registers",

	setRegister(0x9, 27);
	setRegister(0xA, 100);
	setRegister(0xF, 255);

	mu_assert("should set registers correctly", getRegister(0x9) == 27);
	mu_assert("should set registers correctly", getRegister(0xA) == 100);
	mu_assert("should set registers correctly", getRegister(0xF) == 255);

	setFlag(145);
	mu_assert("should set flag register correctly", getFlag() == 145);

	setRegisterI(65500);
	mu_assert("should set register I correctly", getRegisterI() == 65500);
);


BEGIN_SUITE("Arithmetic ops with registers",

	setRegister(0xA, 100);
	setRegister(0xB, 155);
	addRegisters(0xA, 0xB);

	mu_assert("should add registers correctly when it doesn't overflow", getRegister(0xA) == 255);
	mu_assert("should set flag to 0", getFlag() == 0);

	setRegister(0xA, 102);
	setRegister(0xB, 155);
	addRegisters(0xA, 0xB);

	mu_assert("should overflow addition correctly", getRegister(0xA) == 1);
	mu_assert("should set flag to 1", getFlag() == 1);


	//TODO: subtraction

);


BEGIN_SUITE("Shift right with registers",

	setRegister(0xA, 140);
	shiftRight(0xA, 1);

	mu_assert("should do shift right correctly when LS bit is 0", getRegister(0xA) == 70);
	mu_assert("should set flag to 0",  getFlag() == 0);

	setRegister(0xA, 141);
	shiftRight(0xA, 1);

	mu_assert("should do shift right correctly when LS bit is 1", getRegister(0xA) == 70);
	mu_assert("should set flag to 1",  getFlag() == 1);
);


BEGIN_SUITE("Shift left with registers",

	setRegister(0xA, 120);
	shiftLeft(0xA, 1);

	mu_assert("should do shift left correctly when MS bit is 0", getRegister(0xA) == 240);
	mu_assert("should set flag to 0",  getFlag() == 0);

	setRegister(0xA, 130);
	shiftLeft(0xA, 1);

	mu_assert("should do shift left correctly when MS bit is 1", getRegister(0xA) == 4);
	mu_assert("should set flag to 1",  getFlag() == 1);
);


BEGIN_SUITE("Load and store registers",

	int i;
	for(i = 0; i < NUMBER_OF_REGISTERS; i++) {
		setRegister(i, i*3);
	}

	storeRegisters(100, 15);

	for(i = 0; i < NUMBER_OF_REGISTERS; i++) {
		setRegister(i, 0);
	}

	loadRegisters(100, 15);

	int numOfCorrectValues;
	for(i = 0, numOfCorrectValues = 0; i < NUMBER_OF_REGISTERS; i++) {
		int value = getRegister(i);

		if(value == i*3) {
			numOfCorrectValues++;
		}
	}

	mu_assert("should store and load registers to memory correctly", numOfCorrectValues == NUMBER_OF_REGISTERS);

);


BEGIN_SUITE("Random number",

	u8 rnd = generateRandomNumber(MAX_RANDOM_VALUE);

	mu_assert("should be equal or greater than zero", rnd >= 0);
	mu_assert("should be less than 256", rnd < MAX_RANDOM_VALUE);

)

END_TESTS_AUTO_RUN