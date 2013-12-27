#include <stdio.h>
#include <sys/time.h>

#include "cpu.h"
#include "graphics.h"
#include "memory.h"

int main(int argc, char* argv[]) {
	printf("CHIP-8\n");

	initMemory();
	initCPU();
	initGraphics();

	if(argc > 1) {
		if(loadProgram(argv[1]) == 0) {
			printf("Couldn't load program: %s\n", argv[1]);
			return -1;
		}
		else {
			printf("Program loaded");
		}
	}

	printf("updates: %f\n", TIMER_UPDATE_INTERVAL_MILLIS);

	SDL_Delay(2000);

	/*printf("jhd\n");
	Sleep(2000);
	printf("jdsfshd\n");*/

	/*struct timeval currTime;
	gettimeofday(&currTime, NULL);

	printf("%ld\n", currTime.tv_usec);*/
	
	runCPU();

	/*printf("start\n");

	setRegisterI(0);
	setRegister(0xA, 10);
	setRegister(0xB, 10);
	u8 instr[] = {0xDA, 0xB5};
	runInstruction(instr);

	printf("done\n");

	SDL_Delay(2000);

	setRegisterI(5);
	setRegister(0xC, 20);
	setRegister(0xD, 10);
	u8 instr2[] = {0xDC, 0xD5};
	runInstruction(instr2);*/

	//SDL_Delay(2000);

	return 0;
}