#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

using namespace std;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

struct LWindow {
	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;

	int SCREEN_WIDTH = 640;
	int SCREEN_HEIGHT = 480;

	int BUTTON_WIDTH = 12;
	int BUTTON_HEIGHT = 12;
	int TOTAL_BUTTONS = 4;
};

extern LWindow mWindow;

#endif