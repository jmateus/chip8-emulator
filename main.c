#include <stdio.h>
#include <sys/time.h>

#include "cpu.h"
#include "graphics.h"
#include "memory.h"
#include "input.h"

int main(int argc, char* argv[]) {
	printf("CHIP-8\n");

	initMemory();
	initGraphics();
	initInput();
	initCPU();

	if(argc > 1) {
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