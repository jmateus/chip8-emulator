#ifndef CHIP8_INPUT_H
#define CHIP8_INPUT_H

#include <stdbool.h>

#include "types.h"
#include "SDL2/SDL.h"

#define NUM_OF_KEYS 16
#define CHIP8_CONFIG_CONTROLS_KEY "KEY_"


bool isKeyPressed(u4 key);
u4 getKeyPress();

void initInput();

#endif