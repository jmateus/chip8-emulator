#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

#include "types.h"

#define CPU_DEFAULT_FREQ_HZ 1000

#define TIMER_UPDATES_PER_SECOND 60
#define TIMER_UPDATE_INTERVAL_MILLIS (1000.0/TIMER_UPDATES_PER_SECOND)

#define NUMBER_OF_REGISTERS 16
#define REGISTER_MAX_VALUE 255
#define FLAG_REGISTER_INDEX 0xF
#define INSTRUCTION_SIZE 2
#define MAX_RANDOM_VALUE 256

typedef struct {
	u8 registers[NUMBER_OF_REGISTERS]; // V0, V1,..., VE, VF
	u16 I; // registo I

	u8 dt; // delay timer
	u8 st; // sound timer
	float lastUpdate; // last time the timers were updated

	u16 pc; // program counter
	u8 sp; // stack pointer
} CPU;


void setRegister(u4 regist, u8 value);
u8 getRegister(u4 regist);

u8 setFlag(u8 value);
u8 getFlag();

void setRegisterI(u16 value);
u16 getRegisterI();

void runInstruction(u8* instr);
void skipNextInstruction();

void addRegisters(u4 x, u4 y);
void subtractRegisters(u4 target, u4 x, u4 y);

void shiftRight(u8 reg, u8 numOfShifts);
void shiftLeft(u8 reg, u8 numOfShifts);

void storeRegisters(u12 addr, u4 reg);
void loadRegisters(u12 addr, u4 reg);

u8 generateRandomNumber(int maxValue);

CPU* getCPU();

void setClockRate(unsigned int freq);

void stopCPU();
void runCPU();
void initCPU();

#endif