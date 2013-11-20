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

#define INSTRUCTION_SIZE 2

typedef uint8_t u8;
typedef u8 u4;
typedef uint16_t u16;
typedef u16 u12;
typedef unsigned int u32;

typedef u8 reg;

typedef struct {
	reg registers[NUMBER_OF_REGISTERS]; //V0, V1,..., VE, VF
	reg I; //registo I

	reg dt; // delay timer
	reg st; //sound timer

	u16 pc; // program counter
	u8 sp; // stack pointer

	bool skipNext;

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




u8 getStackTop() {
	u8 top = memory[cpu->sp - 1];

	return top;
}



void runCPU() {

	while(cpu->pc < 9) {

		u8* instr = readNextBytes(INSTRUCTION_SIZE);
		u8 high = instr[0];
		u8 low = instr[1];

		u4 lead = (high >> 4) & 0x0F;

		switch(lead) {
			case 0: ;
				u12 number = convertBytesToU12(instr);

				switch(number) {
					case 0x00E0: ;//CLS - clear screen
						//TODO
					printf("CLS\n");

					break; 

					case 0x00EE: ;//RET
						/*u8 ret = getStackTop();
						cpu->pc = ret;
						cpu->sp--;*/

						printf("RET\n");

					break;

					default: ;//system call
						//nop

					printf("SYS CALL\n");
				}

			break;


			default: printf("invalid op: %d\n", instr);


		}

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