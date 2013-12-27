#ifndef CHIP8_GRAPHICS_H
#define CHIP8_GRAPHICS_H

#include <stdbool.h>

#include "types.h"
#include "charset.h"
#include "SDL2/SDL.h"

#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32
#define CHIP8_SCREEN_SIZE (CHIP8_SCREEN_WIDTH * CHIP8_SCREEN_HEIGHT)
#define CHIP8_SCREEN_SCALE 8

#define PIXEL(surface, color) SDL_MapRGB( surface->format, color, color, color )
#define WHITE_PIXEL(surface) PIXEL(surface, 0xFF)
#define BLACK_PIXEL(surface) PIXEL(surface, 0x00)
#define WHITE_PIXEL_BOOL true
#define BLACK_PIXEL_BOOL false

#define WINDOW_TITLE "Chip-8 Emulator"


extern u8 CHIP8_DEFAULT_CHARSET[][DEFAULT_CHARS_SIZE];

typedef struct {
	int width, height, scale;
	SDL_Window *window;
	SDL_Surface *surface;
	bool pixels[CHIP8_SCREEN_WIDTH][CHIP8_SCREEN_HEIGHT];
	bool collision;
} SCREEN;


void drawPixel(int x, int y);
void drawPixelOnScreen(int x, int y, Uint32 color);

void clearScreen();
void updateScreen();

void drawLine(u8 line, u4 x, u4 y);
void drawSprite(u8 *sprite, u4 x, u4 y, u4 size);

bool pixelCollision();
bool isWindowOpen();

void initScreen();
void initGraphics();

#endif