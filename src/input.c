#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "input.h"
#include "../lib/simpleini.h"
#include "SDL2/SDL.h"

SDL_Keycode keys[NUM_OF_KEYS] = {
	SDLK_0,
	SDLK_1,
	SDLK_2,
	SDLK_3,
	SDLK_4,
	SDLK_5,
	SDLK_6,
	SDLK_7,
	SDLK_8,
	SDLK_9,
	SDLK_a,
	SDLK_b,
	SDLK_c,
	SDLK_d,
	SDLK_e,
	SDLK_f
};


bool isKeyPressed(u4 key) {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_GetScancodeFromKey(keys[key])]) {
		return true;
	}

	return false;
}


bool isKeyCodeValid(int keyCode) {
	return keyCode >= 0 && keyCode < NUM_OF_KEYS;
}


u8 getChip8KeyCode(SDL_Keycode key) {
	int keyCode;
	for(keyCode = 0; keyCode < NUM_OF_KEYS; keyCode++) {
		if(keys[keyCode] == key) {
			return keyCode;
		}
	}

	return keyCode;
}


u4 getKeyPress() {
	SDL_Event evt;

	while(true) {
		SDL_WaitEvent(&evt);

		if(evt.type == SDL_QUIT)
			exit(0);

		if(evt.type != SDL_KEYDOWN)
			continue;

		u8 keyCode = getChip8KeyCode(evt.key.keysym.sym);

		if(isKeyCodeValid(keyCode)) {
			return keyCode;
		}
	}
}


void setupInput(INI_NODE* ini) {
	char key[strlen(CHIP8_CONFIG_CONTROLS_KEY) + 2]; //keycode and NULL character
	int keycodeStringIndex = strlen(CHIP8_CONFIG_CONTROLS_KEY); //the keycode is the last character of the string

	int i;
	for(i = 0; i < NUM_OF_KEYS; i++) {
		sprintf(key, CHIP8_CONFIG_CONTROLS_KEY "%x", i);
		key[keycodeStringIndex] = toupper(key[keycodeStringIndex]);

		char* customKey = ini_getValue(ini, key);
		keys[i] = tolower(customKey[0]);
	}
}


void initInput(INI_NODE* ini) {
	SDL_Init(SDL_INIT_VIDEO);
	setupInput(ini);
}
