#ifndef CHIP8_GRAPHICS_H
#define CHIP8_GRAPHICS_H

#include <stdbool.h>

#include "types.h"
#include "SDL2/SDL.h"

#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32
#define CHIP8_SCREEN_SIZE (CHIP8_SCREEN_WIDTH * CHIP8_SCREEN_HEIGHT)
#define CHIP8_SCREEN_SCALE 4

#define DEFAULT_CHARS_SIZE 5

#define PIXEL(surface, color) SDL_MapRGB( surface->format, color, color, color )
#define WHITE_PIXEL(surface) PIXEL(surface, 0xFF)
#define BLACK_PIXEL(surface) PIXEL(surface, 0x00)
#define WHITE_PIXEL_BOOL true
#define BLACK_PIXEL_BOOL false

#define WINDOW_TITLE "Chip-8 Emulator"

const u8 DEFAULT_CHARS[][DEFAULT_CHARS_SIZE] = 
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
void drawSprite(const u8 *sprite, u4 x, u4 y, u4 size);

void initScreen();
void initGraphics();

#endif