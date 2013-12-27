#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include "types.h"

#define MEMORY_SIZE 4096
#define PROGRAM_OFFSET 0x0200
#define STACK_OFFSET 0x0100
#define CHARSET_OFFSET 0x0000

u8 readNextByte(u16* counter);
u8* readNextBytes(u16* counter, u16 numberBytes);

u16 popStack(u8* stack);
void pushStack(u8* stack, u16 value);

u32 convertBytesToU32(u8* bytes, u16 max);
u12 convertBytesToU12(u8* bytes);

u4 getHighU4(u8 number);
u4 getLowU4(u8 number);

void storeToMemory(u12 position, u8 value);
u8 getFromMemory(u12 position);

u8* getMemory(u12 position);
void setMemory(u8* newMemory);

void loadData(u8* data, u12 size, u12 position);
int loadProgram(char* filename);

void loadDefaultCharset(u8* charset, u12 size);
u12 getCharLocation(u4 character);

#endif