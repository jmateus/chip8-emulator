#include "../cpu.h"
#include "minunit.h"

BEGIN_TESTS

initCPU();
initMemory();
CPU* cpu = getCPU();

BEGIN_SUITE("Return instruction",

	pushStack(&cpu->sp, 0x0ABC);

	u8 instr[] = STATIC_INIT({0x00, 0xEE});
	runInstruction(instr);

	mu_assert("should set PC correctly", cpu->pc == 0x0ABC);

);


BEGIN_SUITE("Jump instruction",

	u8 instr[] = STATIC_INIT({0x1D, 0xEF});
	runInstruction(instr);

	mu_assert("should set PC correctly", cpu->pc == 0x0DEF);

);


BEGIN_SUITE("Call instruction",

	cpu->pc = 0x0234;
	cpu->sp = 0;

	u8 instr[] = STATIC_INIT({0x25, 0x67});
	runInstruction(instr);

	mu_assert("should set PC correctly", cpu->pc == 0x0567);
	mu_assert("should increment SP", cpu->sp == 2);
	mu_assert("should stack previous PC", popStack(&cpu->sp) == 0x0234);

);

BEGIN_SUITE("Skip if equal instruction",

	cpu->pc = 0;
	setRegister(0xA, 0x77);

	u8 instr[] = STATIC_INIT({0x3A, 0x77});
	runInstruction(instr);
	mu_assert("should increment PC if equal", cpu->pc == 2);

	cpu->pc = 0;
	instr[0] = 0x3A; instr[1] = 0x78; 
	runInstruction(instr);
	mu_assert("should not increment PC if it isn't equal", cpu->pc == 0);
	
);


BEGIN_SUITE("Skip if not equal instruction",

	cpu->pc = 0;
	setRegister(0xA, 0x77);

	u8 instr[] = STATIC_INIT({0x4A, 0x77});
	runInstruction(instr);
	mu_assert("should not increment PC if equal", cpu->pc == 0);

	cpu->pc = 0;
	instr[0] = 0x4A; instr[1] = 0x78; 
	runInstruction(instr);
	mu_assert("should increment PC if it isn't equal", cpu->pc == 2);
	
);


BEGIN_SUITE("Skip if equal instruction with registers",

	cpu->pc = 0;
	setRegister(0xA, 0x77);
	setRegister(0xB, 0x77);

	u8 instr[] = STATIC_INIT({0x5A, 0xB0});
	runInstruction(instr);
	mu_assert("should increment PC if equal", cpu->pc == 2);

	cpu->pc = 0;
	setRegister(0xB, 0x78);
	runInstruction(instr);
	mu_assert("should not increment PC if it isn't equal", cpu->pc == 0);
	
);


BEGIN_SUITE("Load instruction",

	u8 instr[] = STATIC_INIT({0x6A, 0xBC});
	runInstruction(instr);
	mu_assert("should load value to register", getRegister(0xA) == 0xBC);

);


BEGIN_SUITE("Add instruction",

	setRegister(0xB, 0x95);

	u8 instr[] = STATIC_INIT({0x7B, 0x12});
	runInstruction(instr);
	mu_assert("should add value to register", getRegister(0xB) == 0xA7);

);


BEGIN_SUITE("Load instruction with registers",

	setRegister(0xA, 0x23);
	setRegister(0xB, 0x95);

	u8 instr[] = STATIC_INIT({0x8A, 0xB0});
	runInstruction(instr);
	mu_assert("should load value from one register to another", getRegister(0xA) == 0x95);

);


BEGIN_SUITE("Bitwise operations with registers",

	setRegister(0xA, 0xAB);
	setRegister(0xB, 0xCD);

	u8 instr[] = STATIC_INIT({0x8A, 0xB1});
	runInstruction(instr);
	mu_assert("should do OR correctly", getRegister(0xA) == (0xAB | 0xCD));


	setRegister(0xA, 0xAB);
	setRegister(0xB, 0xCD);

	instr[1] = 0xB2;
	runInstruction(instr);
	mu_assert("should do AND correctly", getRegister(0xA) == (0xAB & 0xCD));


	setRegister(0xA, 0xAB);
	setRegister(0xB, 0xCD);

	instr[1] = 0xB3;
	runInstruction(instr);
	mu_assert("should do XOR correctly", getRegister(0xA) == (0xAB ^ 0xCD));

);


BEGIN_SUITE("Add registers",

	setRegister(0xA, 100);
	setRegister(0xB, 155);
	
	u8 instr[] = STATIC_INIT({0x8A, 0xB4});
	runInstruction(instr);

	mu_assert("should add registers correctly when it doesn't overflow", getRegister(0xA) == 255);
	mu_assert("should set flag to 0", getFlag() == 0);

	setRegister(0xA, 102);
	setRegister(0xB, 155);

	runInstruction(instr);

	mu_assert("should overflow addition correctly", getRegister(0xA) == 1);
	mu_assert("should set flag to 1", getFlag() == 1);

);


BEGIN_SUITE("Subtract registers",

	setRegister(0xA, 200);
	setRegister(0xB, 73);
	
	u8 instr[] = STATIC_INIT({0x8A, 0xB5});
	runInstruction(instr);

	mu_assert("should subtract registers correctly when it doesn't overflow", getRegister(0xA) == 127);
	mu_assert("should set flag to 1", getFlag() == 1);

	setRegister(0xA, 5);
	setRegister(0xB, 6);

	runInstruction(instr);

	mu_assert("should subtract registers correctly when it overflows", getRegister(0xA) == 255);
	mu_assert("should set flag to 0", getFlag() == 0);
);


BEGIN_SUITE("Shift right",

	setRegister(0xA, 140);
	
	u8 instr[] = STATIC_INIT({0x8A, 0xB6});
	runInstruction(instr);

	mu_assert("should do shift right correctly when LS bit is 0", getRegister(0xA) == 70);
	mu_assert("should set flag to 0",  getFlag() == 0);

	setRegister(0xA, 141);
	runInstruction(instr);

	mu_assert("should do shift right correctly when LS bit is 1", getRegister(0xA) == 70);
	mu_assert("should set flag to 1",  getFlag() == 1);
);


BEGIN_SUITE("Shift left",

	setRegister(0xA, 120);

	u8 instr[] = STATIC_INIT({0x8A, 0xBE});
	runInstruction(instr);

	mu_assert("should do shift left correctly when MS bit is 0", getRegister(0xA) == 240);
	mu_assert("should set flag to 0",  getFlag() == 0);

	setRegister(0xA, 130);
	runInstruction(instr);

	mu_assert("should do shift left correctly when MS bit is 1", getRegister(0xA) == 4);
	mu_assert("should set flag to 1",  getFlag() == 1);
);


BEGIN_SUITE("Inverted subtraction with registers",

	setRegister(0xA, 200);
	setRegister(0xB, 73);
	
	u8 instr[] = STATIC_INIT({0x8A, 0xB7});
	runInstruction(instr);

	mu_assert("should subtract registers correctly when it overflows", getRegister(0xA) == 0x81);
	mu_assert("should set flag to 0", getFlag() == 0);

	setRegister(0xA, 5);
	setRegister(0xB, 6);

	runInstruction(instr);

	mu_assert("should subtract registers correctly when it doesn't overflow", getRegister(0xA) == 1);
	mu_assert("should set flag to 1", getFlag() == 1);
);


BEGIN_SUITE("Skip if not equal instruction with registers",

	cpu->pc = 0;
	setRegister(0xA, 0x77);
	setRegister(0xB, 0x77);

	u8 instr[] = STATIC_INIT({0x9A, 0xB0});
	runInstruction(instr);
	mu_assert("should not increment PC if equal", cpu->pc == 0);

	cpu->pc = 0;
	setRegister(0xB, 0x78);
	runInstruction(instr);
	mu_assert("should increment PC if it isn't equal", cpu->pc == 2);
	
);


BEGIN_SUITE("Load instruction with register I",

	setRegisterI(0);

	u8 instr[] = STATIC_INIT({0xA8, 0x9A});
	runInstruction(instr);
	mu_assert("should load value to register I", getRegisterI() == 0x089A);

);


BEGIN_SUITE("Jump instruction with register",

	setRegister(0x0, 0x12);

	u8 instr[] = STATIC_INIT({0xB6, 0x74});
	runInstruction(instr);

	mu_assert("should set PC correctly", cpu->pc == 0x0686);

);


BEGIN_SUITE("Random number",

	u8 instr[] = STATIC_INIT({0xCA, 0xAA});
	runInstruction(instr);

	u8 rnd = getRegister(0xA);
	printf("rnd: %x\n", rnd);

	mu_assert("should be equal or greater than zero", rnd >= 0);
	mu_assert("should be less than 256", rnd < MAX_RANDOM_VALUE);
	mu_assert("should be ANDed with kk", (rnd & (0xAA ^ 0xFF)) == 0x00);

);


BEGIN_SUITE("Load value from DT",

	cpu->dt = 0x9A;

	u8 instr[] = STATIC_INIT({0xFA, 0x07});
	runInstruction(instr);
	mu_assert("should load value from DT to register", getRegister(0xA) == 0x9A);

);


BEGIN_SUITE("Set value of DT",

	setRegister(0xB, 0x55);

	u8 instr[] = STATIC_INIT({0xFB, 0x15});
	runInstruction(instr);
	mu_assert("should load value from register to DT", cpu->dt == 0x55);

);


BEGIN_SUITE("Set value of ST",

	setRegister(0xA, 0x77);

	u8 instr[] = STATIC_INIT({0xFA, 0x18});
	runInstruction(instr);
	mu_assert("should load value from register to ST", cpu->st == 0x77);

);


BEGIN_SUITE("Add register with I",

	setRegisterI(0xAB);
	setRegister(0xB, 0x33);
	
	u8 instr[] = STATIC_INIT({0xFB, 0x1E});
	runInstruction(instr);

	mu_assert("should add register and I, storing result in I", getRegisterI() == 0xDE);

);


BEGIN_SUITE("BCD representation",

	setRegisterI(0x100);
	setRegister(0xA, 234);
	
	u8 instr[] = STATIC_INIT({0xFA, 0x33});
	runInstruction(instr);

	mu_assert("should store hundreads digit", getFromMemory(0x100) == 2);
	mu_assert("should store tens digit", getFromMemory(0x101) == 3);
	mu_assert("should store units digit", getFromMemory(0x102) == 4);

);


BEGIN_SUITE("Load and store registers",

	setRegisterI(0x100);
	int numOfRegistersStored = 0xB;

	u16 safetyIndex = getRegisterI() + numOfRegistersStored + 1;
	u8 safetyVal = getFromMemory(safetyIndex);

	int i;
	for(i = 0; i < numOfRegistersStored; i++) {
		setRegister(i, i*3);
	}

	u8 instr[] = STATIC_INIT({0xFF, 0x55});
	runInstruction(instr);

	for(i = 0; i < numOfRegistersStored; i++) {
		setRegister(i, 0);
	}

	instr[1] = 0x65;
	runInstruction(instr);

	int numOfCorrectValues;
	for(i = 0, numOfCorrectValues = 0; i < numOfRegistersStored; i++) {
		int value = getRegister(i);

		if(value == i*3) {
			numOfCorrectValues++;
		}
	}

	mu_assert("should store and load registers to memory correctly", numOfCorrectValues == numOfRegistersStored);
	mu_assert("should not overflow", getFromMemory(safetyIndex) == safetyVal);

);


END_TESTS_AUTO_RUN