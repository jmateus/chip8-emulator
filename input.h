#ifndef CHIP8_INPUT_H
#define CHIP8_INPUT_H

#include <stdbool.h>

#include "types.h"
#include "SDL2/SDL.h"

#define NUM_OF_KEYS 16

extern SDL_Keycode KEY_CODES[NUM_OF_KEYS];
extern SDL_Scancode KEY_SCANS[NUM_OF_KEYS];

bool isKeyPressed(u4 key);
u4 getKeyPress();

void initInput();

#endif