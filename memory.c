#include <stdlib.h>

#include "memory.h"


static u8* memory;

u8 readNextByte(u16* counter) {
	u8 next = memory[*counter];
	(*counter)++;

	return next;
}


u8* readNextBytes(u16* counter, u16 numberBytes) {
	u8* bytes = (u8*) malloc(sizeof(u8) * numberBytes);

	int i;
	for(i = 0; i < numberBytes; i++) {
		bytes[i] = readNextByte(counter);
	}

	return bytes;
}


u16 popStack(u8* stack) {
	u16 top = 0;

	top += memory[*stack-2];
	top = (top << 8) & 0xFF00;
	top += memory[*stack-1];

	*stack -= 2;

	return top;
}


void pushStack(u8* stack, u16 value) {
	u8 high = (value >> 8) & 0x00FF;
	u8 low = value & 0x00FF;

	memory[*stack] = high;
	memory[*stack + 1] = low;

	*stack += 2;
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


void storeToMemory(u12 position, u8 value) {
	memory[position] = value;
}


u8 getFromMemory(u12 position) {
	return memory[position];
}


void initMemory() {
	memory = (u8*) malloc(sizeof(u8) * MEMORY_SIZE);

	memory[0] = 0x00;
	memory[1] = 0xEE;
	memory[2] = 0x00;
	memory[3] = 0xEE;
	memory[4] = 0x00;
	memory[5] = 0xE0;
	memory[6] = 0x01;
	memory[7] = 0x0F;
	memory[8] = 0xFF;
}