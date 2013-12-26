#include <stdio.h>

#include "cpu.h"

int main(int argc, char* argv[]) {
	printf("CHIP-8\n");

	initCPU();
	runCPU();

	return 0;
}