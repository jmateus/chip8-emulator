#include <stdio.h>
#include <stdlib.h>

#include "input.h"
#include "SDL2/SDL.h"

SDL_Keycode KEYS[NUM_OF_KEYS] = {
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

SDL_Scancode KEY_SCANS[NUM_OF_KEYS] = {
	SDL_SCANCODE_0,
	SDL_SCANCODE_1,
	SDL_SCANCODE_2,
	SDL_SCANCODE_3,
	SDL_SCANCODE_4,
	SDL_SCANCODE_5,
	SDL_SCANCODE_6,
	SDL_SCANCODE_7,
	SDL_SCANCODE_8,
	SDL_SCANCODE_9,
	SDL_SCANCODE_A,
	SDL_SCANCODE_B,
	SDL_SCANCODE_C,
	SDL_SCANCODE_D,
	SDL_SCANCODE_E,
	SDL_SCANCODE_F
};


bool isKeyPressed(u4 key) {
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[KEY_SCANS[key]]) {
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
		if(KEYS[keyCode] == key) {
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


void initInput() {
	SDL_Init(SDL_INIT_VIDEO); //input is in the video module of SDL
}
