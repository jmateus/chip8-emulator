#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"
#include "SDL2/SDL.h"


u8 CHIP8_DEFAULT_CHARSET[][DEFAULT_CHARS_SIZE] = 
{
	{0xF0, 0x90, 0x90, 0x90, 0xF0}, // 0
	{0x20, 0x60, 0x20, 0x20, 0x70}, // 1
	{0xF0, 0x10, 0xF0, 0x80, 0xF0}, // 2
	{0xF0, 0x10, 0xF0, 0x10, 0xF0}, // 3
	{0x90, 0x90, 0xF0, 0x10, 0x10}, // 4
	{0xF0, 0x80, 0xF0, 0x10, 0xF0}, // 5
	{0xF0, 0x80, 0xF0, 0x90, 0xF0}, // 6
	{0xF0, 0x10, 0x20, 0x40, 0x40}, // 7
	{0xF0, 0x90, 0xF0, 0x90, 0xF0}, // 8
	{0xF0, 0x90, 0xF0, 0x10, 0xF0}, // 9
	{0xF0, 0x90, 0xF0, 0x90, 0x90}, // A
	{0xE0, 0x90, 0xE0, 0x90, 0xE0}, // B
	{0xF0, 0x80, 0x80, 0x80, 0xF0}, // C
	{0xE0, 0x90, 0x90, 0x90, 0xE0}, // D
	{0xF0, 0x80, 0xF0, 0x80, 0xF0},	// E
	{0xF0, 0x80, 0xF0, 0x80, 0x80} 	// F
};


static SCREEN *screen;

void drawPixel(int x, int y) {

	int xCoord = x % CHIP8_SCREEN_WIDTH;
	int yCoord = y % CHIP8_SCREEN_HEIGHT;

	Uint32 color;

	if(screen->pixels[xCoord][yCoord]) {
		screen->collision = true;
		color = BLACK_PIXEL(screen->surface);
		screen->pixels[xCoord][yCoord] = BLACK_PIXEL_BOOL;
	}
	else {
		color = WHITE_PIXEL(screen->surface);
		screen->pixels[xCoord][yCoord] = WHITE_PIXEL_BOOL;
	}

	int i, j;

	for(i = 0; i < screen->scale; i++)
		for(j = 0; j < screen->scale; j++) {
			drawPixelOnScreen(xCoord*screen->scale + i, yCoord*screen->scale + j, color);
		}
}


void drawPixelOnScreen(int x, int y, Uint32 color) {
	int xCoord = x % screen->width;
	int yCoord = y % screen->height;

	Uint32* pixels = (Uint32*) screen->surface->pixels;

	//TODO: investigar porque
	pixels[yCoord*((screen->surface->pitch / 4)) + xCoord] = color;
}


void clearScreen() {
	SDL_FillRect(screen->surface, NULL, BLACK_PIXEL(screen->surface));
	SDL_UpdateWindowSurface(screen->window);

	int i, j;
	for(i = 0; i < CHIP8_SCREEN_WIDTH; i++) {
		for(j = 0; j < CHIP8_SCREEN_HEIGHT; j++) {
			screen->pixels[i][j] = BLACK_PIXEL_BOOL;
		}
	}
}


void updateScreen() {
	SDL_UpdateWindowSurface(screen->window);
}


void drawLine(u8 line, u4 x, u4 y) {
	int length = sizeof(u8) * 8;

	int i;
	for(i = 0; i < length; i++) {
		int draw = (line >> (7-i)) & 0x01;

		if(draw) {
			drawPixel(x+i, y);
		}
	}
}


void drawSprite(u8 *sprite, u4 x, u4 y, u4 size) {

	SDL_LockSurface(screen->surface);

	screen->collision = false;

	int i;
	for(i = 0; i < size; i++) {
		drawLine(sprite[i], x, y+i);
	}

	SDL_UnlockSurface(screen->surface);

	SDL_UpdateWindowSurface(screen->window);
}


bool pixelCollision() {
	return screen->collision;
}


bool isWindowOpen() {
	SDL_Event evt;

	SDL_PollEvent(&evt);
	if(evt.type == SDL_QUIT) {
		return false;
	}
	else {
		return true;
	}
}


void initScreen() {
	screen = (SCREEN*) calloc(1, sizeof(SCREEN));

	screen->width = CHIP8_SCREEN_WIDTH * CHIP8_SCREEN_SCALE;
	screen->height = CHIP8_SCREEN_HEIGHT * CHIP8_SCREEN_SCALE;
	screen->scale = CHIP8_SCREEN_SCALE;

	screen->window = SDL_CreateWindow(
		WINDOW_TITLE,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		screen->width,
		screen->height,
		SDL_WINDOW_SHOWN
	);

	if (screen->window == NULL) {
		printf("Could not create window: %s\n", SDL_GetError());
		exit(1);
	}
	else {
		screen->surface = SDL_GetWindowSurface(screen->window);
	}

	screen->collision = false;

	clearScreen();
}

void initGraphics() {

	SDL_Init(SDL_INIT_EVERYTHING); //TODO: init only video
	initScreen();

}