#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

using namespace std;

// 0 or 10: blank cell
// 1 to 8: numbered cells
// 9: default cell
// 11: bomb cell
// 12: flag cell
// 14: explosion cell
// 15: question mark cell
// 13, 16, 17: not used
enum LButtonSprite
{
	BUTTON_SPRITE_DEFAULT = 9,
	// Blank, numbered cells
	BUTTON_SPRITE_ZERO = 0,
	BUTTON_SPRITE_ONE = 1,
	BUTTON_SPRITE_TWO = 2,
	BUTTON_SPRITE_THREE = 3,
	BUTTON_SPRITE_FOUR = 4,
	BUTTON_SPRITE_FIVE = 5,
	BUTTON_SPRITE_SIX = 6,
	BUTTON_SPRITE_SEVEN = 7,
	BUTTON_SPRITE_EIGHT = 8,
	// Special cells
	BUTTON_SPRITE_BOMB = 11,
	BUTTON_SPRITE_FLAG = 12,
	BUTTON_SPRITE_EXPLOSION = 14,
	BUTTON_SPRITE_FACE = 31,
	BUTTON_SPRITE_LOSE_FACE = 33,
	BUTTON_SPRITE_WIN_FACE = 34,
	BUTTON_SPRITE_TOTAL = 54 // 18 sprites for buttons (need to be loaded)
};

enum LSize {
	CELL_SIZE = 16
};

struct LWindow {
	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	int GAME_WIDTH;
	int GAME_HEIGHT;
	int BOMBS;
	int TOTAL_BUTTONS; // Size for game
};

extern LWindow mWindow;

#endif