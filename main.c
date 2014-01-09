#include <stdio.h>

#include "cpu.h"
#include "graphics.h"
#include "memory.h"
#include "input.h"

#include "lib/simpleini.h"

#define CHIP8_CONFIG_FILE "config.ini"
#define CHIP8_CONFIG_SCALE_KEY "SCALE"
#define CHIP8_CONFIG_CPU_FREQ_KEY "CPU_CLOCK_RATE"


void startSystem() {
	INI_NODE* ini = ini_initSimpleIni(CHIP8_CONFIG_FILE);

	int scale = ini_getInt(ini, CHIP8_CONFIG_SCALE_KEY, 10);
	int freq = ini_getInt(ini, CHIP8_CONFIG_CPU_FREQ_KEY, 10);

	initMemory();
	initGraphics(scale);
	initInput();
	initCPU();

	setClockRate(freq);
}


int main(int argc, char* argv[]) {
	printf("Chip-8 Emulator (2013)\n");

	startSystem();

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
			printf("Program loaded: %s\n", argv[1]);
		}
	}
	
	runCPU();

	return 0;
}