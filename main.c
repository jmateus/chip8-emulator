#include <stdio.h>

#include "cpu.h"

int main(int argc, char* argv[]) {
	printf("CHIP-8\n");

	initCPU();
	initMemory();
	initGraphics();

	runCPU();

	return 0;
}