#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "charset.h"

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

	top += memory[STACK_OFFSET + (*stack-2)];
	top = (top << 8) & 0xFF00;
	top += memory[STACK_OFFSET + (*stack-1)];

	*stack -= 2;

	return top;
}


void pushStack(u8* stack, u16 value) {
	u8 high = (value >> 8) & 0x00FF;
	u8 low = value & 0x00FF;

	memory[STACK_OFFSET + (*stack)] = high;
	memory[STACK_OFFSET + (*stack + 1)] = low;

	*stack += 2;
}


u32 convertBytesToU32(u8* bytes, u16 max) {
	u32 number = 0;

	int i;
	for(i = 0; i < max; i++) {
		number = (number << 8) & 0xFFFFFF00;
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


u8* getMemory(u12 position) {
	return &memory[position];
}


//TODO: untested
void loadData(u8* data, u12 size, u12 position) {
	memcpy(&memory[position], data, size);
}


//TODO: untested
int loadProgram(char* filename) {
	FILE* file = fopen(filename, "rb");

	if(file != NULL) {
		int i;
		for(i = 0; i < MEMORY_SIZE && !feof(file); i++) {
			fread(&memory[PROGRAM_OFFSET + i], sizeof(u8), 1, file);
		}

		return i;
	}

	return 0;
}


//TODO: untested
void loadDefaultCharset(u8* charset, u12 size) {
	loadData(charset, size, CHARSET_OFFSET);
}


//TODO: untested
u12 getCharLocation(u4 character) {
	return CHARSET_OFFSET + (character * DEFAULT_CHARS_SIZE);
}


void initMemory() {
	memory = (u8*) calloc(1, sizeof(u8) * MEMORY_SIZE);
}