#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MEMORY_SIZE 4096
#define SCREEN_SIZE_BYTES 256
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define PROGRAM_OFFSET 0x200
#define STACK_OFFSET 0
#define NUMBER_OF_REGISTERS 16
#define CPU_FREQ 60
#define REGISTER_MAX_VALUE 255
#define FLAG_REGISTER_INDEX 0xF

#define INSTRUCTION_SIZE 2

typedef uint8_t u8;
typedef u8 u4;
typedef uint16_t u16;
typedef u16 u12;
typedef unsigned int u32;

typedef struct {
	u8 registers[NUMBER_OF_REGISTERS]; //V0, V1,..., VE, VF
	u8 I; //registo I

	u8 dt; // delay timer
	u8 st; //sound timer

	u16 pc; // program counter
	u8 sp; // stack pointer

} CPU;


CPU* cpu;
u8* memory;
u8* screen;


u8 readNextByte() {
	u8 next = memory[cpu->pc];
	cpu->pc++;

	return next;
}


u16 popStack() {
	u16 top = 0;

	top += memory[cpu->sp-2];
	top = (top << 8) & 0xFF00;
	top += memory[cpu->sp-1];

	cpu->sp -= 2;

	return top;
}

void pushStack(u16 value) {
	u8 high = (value >> 8) & 0x00FF;
	u8 low = value & 0x00FF;

	memory[cpu->sp] = high;
	memory[cpu->sp + 1] = low;

	cpu->sp += 2;
}



u8* readNextBytes(u16 numberBytes) {
	u8* bytes = (u8*) malloc(sizeof(u8) * numberBytes);

	int i;
	for(i = 0; i < numberBytes; i++) {
		bytes[i] = readNextByte();
	}

	return bytes;

}

u32 convertBytesToU32(u8* bytes, u16 max) {

	u32 number = 0;

	int i;
	for(i = 0; i < max; i++) {
		number = (number << 8*i) & 0xFFFFFF00;
		number += bytes[i];
	}

	return number;

}


u12 convertBytesToU12(u8* bytes) {

	u12 number = 0;

	number += bytes[0];
	number = (number << 8) & 0x0F00;
	number += bytes[1];

	return number;
}

u4 getHighU4(u8 number) {
	u4 high = (number >> 4) & 0x0F;
	return high; 
}

u4 getLowU4(u8 number) {
	u4 low = number & 0x0F;
	return low; 
}

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



void runCPU() {

	while(cpu->pc < 9) {

		u8* instr = readNextBytes(INSTRUCTION_SIZE);
		u8 high = instr[0];
		u8 low = instr[1];

		u4 lead = (high >> 4) & 0x0F;

		switch(lead) {
			case 0: ;
			{
				u12 number = convertBytesToU12(instr);

				switch(number) {
					case 0x00E0: ;//CLS - clear screen
						//TODO
						printf("CLS\n");

						break; 

					case 0x00EE: ;//RET
						u16 ret = popStack();
						cpu->pc = ret;

						printf("RET\n");

						break;

					default: ;//system call
						//nop

					printf("SYS CALL\n");
				}


				break;

			}

			case 1: ; //JUMP addr (1nnn)
			{
				u12 addr = convertBytesToU12(instr);
				cpu->pc = addr;

				break;
			}

			case 2: ; //CALL addr (2nnn)
			{
				pushStack(cpu->pc);
				u12 addr = convertBytesToU12(instr);
				cpu->pc = addr;

				break;
			}

			case 3: ; //SKIP EQUAL (3xkk)
			{
				u4 regIndex = getLowU4(high);
				u8 regValue = getRegister(regIndex);

				if(regValue == low) {
					skipNextInstruction();
				}

				break;
			}

			case 4: ; //SKIP NOT EQUAL (4xkk)
			{
				u4 regIndex = getLowU4(high);
				u8 regValue = getRegister(regIndex);

				if(regValue != low) {
					skipNextInstruction();
				}

				break;
			}

			case 5: ; // SKIP EQUAL (5xy0)
			{
				u4 firstRegIndex = getLowU4(high);
				u8 firstRegValue = getRegister(firstRegIndex);

				u4 secondRegIndex = getHighU4(low);
				u8 secondRegValue = getRegister(secondRegValue);


				if(firstRegValue == secondRegValue) {
					skipNextInstruction();
				}


				break;
			}

			case 6: ; // LOAD (6xkk)
			{
				u4 regIndex = getLowU4(high);

				setRegister(regIndex, low);

				break;
			}

			case 7: ; // ADD (7xkk)
			{
				u4 regIndex = getLowU4(high);
				u8 regValue = getRegister(regIndex);

				setRegister(regIndex, regValue + low);

				break;
			}

			case 8: ; // (8xyk)
			{
				u4 firstRegIndex = getLowU4(high);
				u8 firstRegValue = getRegister(firstRegIndex);

				u4 secondRegIndex = getHighU4(low);
				u8 secondRegValue = getRegister(secondRegValue);


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

			case 9: ; // SKIP NOT EQUAL (9xy0)
			{
				u4 firstRegIndex = getLowU4(high);
				u8 firstRegValue = getRegister(firstRegIndex);

				u4 secondRegIndex = getHighU4(low);
				u8 secondRegValue = getRegister(secondRegValue);


				if(firstRegValue != secondRegValue) {
					skipNextInstruction();
				}

				break;
			}

			default: printf("invalid op: %d\n", instr);


		}

		free(instr);

	}


}


int main(int argc, char* argv[]) {
	printf("CHIP-8\n");

	cpu = (CPU*) malloc(sizeof(CPU));
	cpu->pc = 0;
	cpu->sp = 200;

	memory = (u8*) malloc(sizeof(u8) * MEMORY_SIZE);
	screen = (u8*) malloc(sizeof(u8) * SCREEN_SIZE_BYTES);

	memory[0] = 0x00;
	memory[1] = 0xEE;
	memory[2] = 0x00;
	memory[3] = 0xEE;
	memory[4] = 0x00;
	memory[5] = 0xE0;
	memory[6] = 0x01;
	memory[7] = 0x0F;
	memory[8] = 0xFF;

	runCPU();

	printf("skipNext: %d\n", sizeof(u8) * MEMORY_SIZE);

	return 0;


}