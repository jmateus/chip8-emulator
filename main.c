#include <stdio.h>

#include "cpu.h"
#include "graphics.h"
#include "memory.h"
#include "input.h"

int main(int argc, char* argv[]) {
	printf("Chip-8 Emulator (2013)\n");

	initMemory();
	initGraphics();
	initInput();
	initCPU();

	if(argc < 2) {
		printf("Please specify a program to run: %s [program file]\n", argv[0]);
		return -1;
	}
	else {
		if(loadProgram(argv[1]) == 0) {
			printf("Couldn't load program: %s\n", argv[1]);
			return -1;
		}
		else {
			printf("Program loaded\n");
		}
	}
	
	runCPU();

	return 0;
}