#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "cpu.h"
#include "types.h"
#include "memory.h"
#include "graphics.h"


static CPU *cpu;


void setRegister(u4 regist, u8 value) {
	cpu->registers[regist] = value;
}

u8 getRegister(u4 regist) {
	return cpu->registers[regist];
}

void skipNextInstruction() {
	cpu->pc += INSTRUCTION_SIZE;
}

u8 setFlag(u8 value) {
	u8 oldValue = getRegister(FLAG_REGISTER_INDEX);
	setRegister(FLAG_REGISTER_INDEX, value);

	return oldValue;
}

u8 getFlag() {
	return getRegister(FLAG_REGISTER_INDEX);
}

void setRegisterI(u16 value) {
	cpu->I = value;
}

u16 getRegisterI() {
	return cpu->I;
}


void addRegisters(u4 x, u4 y){
	u8 xValue = getRegister(x);
	u8 yValue = getRegister(y);

	u16 result = xValue + yValue;

	if(result > REGISTER_MAX_VALUE) {
		setFlag(1);
	}
	else {
		setFlag(0);
	}

	u8 u8result = result & 0x00FF;
	setRegister(x, u8result);
}


void subtractRegisters(u4 target, u4 x, u4 y){
	u8 xValue = getRegister(x);
	u8 yValue = getRegister(y);

	u8 result = xValue - yValue;

	if(xValue > yValue) {
		setFlag(1);
	}
	else {
		setFlag(0);
	}

	setRegister(target, result);
}

void shiftRight(u8 reg, u8 numOfShifts) {
	u8 regValue = getRegister(reg);

	u8 newValue = regValue >> numOfShifts;

	if((regValue & 0x01) == 1) {
		setFlag(1);
	}
	else {
		setFlag(0);
	}

	setRegister(reg, newValue);
}

void shiftLeft(u8 reg, u8 numOfShifts) {
	u8 regValue = getRegister(reg);

	u8 newValue = regValue << numOfShifts;

	if(((regValue & 0x80) >> 7) == 1) { //TODO: test
		setFlag(1);
	}
	else {
		setFlag(0);
	}

	setRegister(reg, newValue);
}



void storeRegisters(u12 addr, u4 reg) {
	int i;
	for(i = 0; i <= reg; i++) {
		u8 regValue = getRegister(i);
		storeToMemory(addr + i, regValue);
	}
}

void loadRegisters(u12 addr, u4 reg) {
	int i;
	for(i = 0; i <= reg; i++) {
		u8 value = getFromMemory(addr + i);
		setRegister(i, value);
	}
}

u8 generateRandomNumber(int maxValue) {
	u8 rnd = rand() % maxValue;
	return rnd;
}


void updateTimers() {
	clock_t currTime = clock();
	float currTimeMillis = (((float)currTime)/CLOCKS_PER_SEC) * 1000;

	if((currTimeMillis - cpu->lastUpdate) > TIMER_UPDATE_INTERVAL_MILLIS) {
		cpu->dt = cpu->dt == 0 ? 0 : cpu->dt - 1;
		cpu->st = cpu->st == 0 ? 0 : cpu->st - 1;
		cpu->lastUpdate = currTimeMillis;

		//printf("update timers\n");
	}
}


CPU* getCPU() {
	return cpu;
}


void runInstruction(u8* instr) {

	u8 high = instr[0];
	u8 low = instr[1];

	u4 lead = getHighU4(high);

	switch(lead) {
		case 0x0: ;
		{
			u12 number = convertBytesToU12(instr);

			switch(number) {
				case 0x00E0: ; //CLS (00E0)
					//TODO: test

					clearScreen();

					//printf("CLS\n");

					break; 

				case 0x00EE: ; //RET
					u16 ret = popStack(&cpu->sp);
					cpu->pc = ret;

					//printf("RET\n");

					break;

				default: ; //system call
					//nop
					//printf("SYS CALL\n");
			}


			break;

		}

		case 0x1: ; //JUMP addr (1nnn)
		{
			u12 addr = convertBytesToU12(instr);
			cpu->pc = addr;

			break;
		}

		case 0x2: ; //CALL addr (2nnn)
		{
			pushStack(&cpu->sp, cpu->pc);
			u12 addr = convertBytesToU12(instr);
			cpu->pc = addr;

			break;
		}

		case 0x3: ; //SKIP EQUAL (3xkk)
		{
			u4 regIndex = getLowU4(high);
			u8 regValue = getRegister(regIndex);

			if(regValue == low) {
				skipNextInstruction();
			}

			break;
		}

		case 0x4: ; //SKIP NOT EQUAL (4xkk)
		{
			u4 regIndex = getLowU4(high);
			u8 regValue = getRegister(regIndex);

			if(regValue != low) {
				skipNextInstruction();
			}

			break;
		}

		case 0x5: ; // SKIP EQUAL (5xy0)
		{
			u4 firstRegIndex = getLowU4(high);
			u8 firstRegValue = getRegister(firstRegIndex);

			u4 secondRegIndex = getHighU4(low);
			u8 secondRegValue = getRegister(secondRegIndex);

			if(firstRegValue == secondRegValue) {
				skipNextInstruction();
			}

			break;
		}

		case 0x6: ; // LOAD (6xkk)
		{
			u4 regIndex = getLowU4(high);

			setRegister(regIndex, low);

			break;
		}

		case 0x7: ; // ADD (7xkk)
		{
			u4 regIndex = getLowU4(high);
			u8 regValue = getRegister(regIndex);

			setRegister(regIndex, regValue + low);

			break;
		}

		case 0x8: ; // (8xyk)
		{
			u4 firstRegIndex = getLowU4(high);
			u8 firstRegValue = getRegister(firstRegIndex);

			u4 secondRegIndex = getHighU4(low);
			u8 secondRegValue = getRegister(secondRegIndex);


			u4 op = getLowU4(low);

			switch(op) {

				case 0x0: ; // LOAD

					setRegister(firstRegIndex, secondRegValue);
					break;

				case 0x1: ; // OR

					setRegister(firstRegIndex, firstRegValue | secondRegValue);
					break;

				case 0x2: ; // AND

					setRegister(firstRegIndex, firstRegValue & secondRegValue);
					break;

				case 0x3: ; // XOR

					setRegister(firstRegIndex, firstRegValue ^ secondRegValue);
					break;

				case 0x4: ; // ADD

					addRegisters(firstRegIndex, secondRegIndex);
					break;

				case 0x5: ; // SUB (x-y)

					subtractRegisters(firstRegIndex, firstRegIndex, secondRegIndex);
					break;

				case 0x6: ; // SHR 1

					shiftRight(firstRegIndex, 1);
					break;

				case 0x7: ; // SUBN (y-x)

					subtractRegisters(firstRegIndex, secondRegIndex, firstRegIndex);
					break;

				case 0xE: ; // SHL 1

					shiftLeft(firstRegIndex, 1);
					break;

				default: ; //invalid op

					printf("[ERROR] Invalid op for instruction 0x8xxx\n");

			}

			break;
		}

		case 0x9: ; // SKIP NOT EQUAL (9xy0)
		{
			u4 firstRegIndex = getLowU4(high);
			u8 firstRegValue = getRegister(firstRegIndex);

			u4 secondRegIndex = getHighU4(low);
			u8 secondRegValue = getRegister(secondRegIndex);


			if(firstRegValue != secondRegValue) {
				skipNextInstruction();
			}

			break;
		}

		case 0xA: ; // LOAD I, addr (Annn)
		{
			u12 addr = convertBytesToU12(instr);
			cpu->I = addr;

			break;
		}

		case 0xB: ; // JUMP V0 + nnn (Bnnn)
		{
			u12 addr = convertBytesToU12(instr);
			u16 newAddr = addr + getRegister(0);
			cpu->pc = newAddr;

			break;
		}

		case 0xC: ; // Vx = random byte AND kk (Cxkk)
		{
			u8 rnd = generateRandomNumber(MAX_RANDOM_VALUE);
			u4 regIndex = getLowU4(high);
			setRegister(regIndex, rnd & low);

			break;
		}

		case 0xD: ; // Draw Vx, Vy, n (Dxyn)
		{
			u12 spriteLocation = getRegisterI();
			u8* sprite = getMemory(spriteLocation);

			u4 firstRegIndex = getLowU4(high);
			u8 xCoord = getRegister(firstRegIndex);

			u4 secondRegIndex = getHighU4(low);
			u8 yCoord = getRegister(secondRegIndex);

			u4 spriteSize = getLowU4(low);

			drawSprite(sprite, xCoord, yCoord, spriteSize);

			break;
		}

		case 0xE: ; // Input
		{
			u4 regIndex = getLowU4(high);

			switch(low) {
				case 0x9E: ; // SKIP if Vx is pressed
				{
					//TODO

					break;
				}

				case 0xA1: ; //SKIP if Vx is not pressed
				{
					//TODO

					break;
				}

				default: ; //invalid input op
					printf("[ERROR] Invalid op for instruction 0xExxx\n");
			}

			break;
		}

		case 0xF: ; // Fxkk
		{
			u4 reg = getLowU4(high);

			switch(low) {
				case 0x07: ; // LOAD Vx, DT
				{
					setRegister(reg, cpu->dt);
					break;
				}

				case 0x0A: ; // LOAD Vx, K
				{
					//TODO

					printf("Waiting input...\n");

					SDL_Delay(2000);

					break;
				}

				case 0x15: ; // LOAD DT, Vx
				{
					cpu->dt = getRegister(reg);
					break;
				}

				case 0x18: ; // LOAD ST, Vx
				{
					cpu->st = getRegister(reg);
					break;
				}

				case 0x1E: ; // ADD I, Vx
				{
					u16 result = cpu->I + getRegister(reg);
					cpu->I = result;
					break;
				}

				case 0x29: ; // LD F, Vx
				{
					//TODO: test

					u4 character = getRegister(reg);
					u12 charLocation = getCharLocation(character);

					setRegisterI(charLocation);

					break;
				}

				case 0x33: ; // LD B, Vx
				{
					u8 regValue = getRegister(reg);
					u8 hundreads = regValue / 100;
					u8 tens = (regValue % 100) / 10;
					u8 units = regValue % 10;

					u12 I = cpu->I;
					storeToMemory(I, hundreads);
					storeToMemory(I+1, tens);
					storeToMemory(I+2, units);

					break;
				}

				case 0x55: ; // LD [I], Vx
				{
					storeRegisters(cpu->I, reg);
					break;
				}

				case 0x65: ; // LD Vx, [I]
				{
					loadRegisters(cpu->I, reg);
					break;
				}
			}

			break;
		}

		default: printf("invalid op: %d\n", instr);
	}
}



//TODO
void runCPU() {

	SDL_Event evt;

	while(true) {
		u8* instr = readNextBytes(&cpu->pc, INSTRUCTION_SIZE);

		//printf("Instruction: %X%X\n", instr[0], instr[1]);

		runInstruction(instr);
		free(instr);

		updateTimers();
		SDL_Delay(CPU_INTERVAL_BETWEEN_OPS_MILLIS);

		SDL_PollEvent(&evt);
  		if(evt.type == SDL_QUIT)
    		break;
	}
}


void initCPU() {
	cpu = (CPU*) calloc(1, sizeof(CPU));
	cpu->pc = PROGRAM_OFFSET;

	loadDefaultCharset((u8*) CHIP8_DEFAULT_CHARSET, DEFAULT_CHARSET_SIZE);

	srand(time(NULL));
}
