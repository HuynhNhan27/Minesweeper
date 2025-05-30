#ifndef MAKER_H
#define MAKER_H

#include "utils.h"
#include "LTexture.h"
#include "LButton.h"

class Maker {
private:
	//Mouse button sprites
	SDL_Rect* gSpriteClips = NULL;
	LTexture gButtonSpriteSheetTexture;

	//Buttons objects
	LButton** gButtons = NULL;

	//Background texture
	LButton* background = NULL;

	int BUTTON_WIDTH = 15;
	int BUTTON_HEIGHT = 15;
	int TIMER_WIDTH = 11;
	int TIMER_HEIGHT = 21;
	int FACE_WIDTH = 26;
	int FACE_HEIGHT = 26;
	int CORNER_WIDTH = 12;
	int CORNER_HEIGHT = 11;
	int BORDER_WIDTH = 1;
	int BORDER_HEIGHT = 11;
	int TIMER_WINDOW_WIDTH = 41;
	int TIMER_WINDOW_HEIGHT = 25;

	int win_condition = 0;
public:
	Maker(int width, int height, int bombs);
	~Maker();

	//Initialize the maker
	bool init();

	//Load media for the maker
	bool loadMedia();

	bool renderBackground();

	//Free resources and close SDL
	void close();

	void renderAll();

	//Run the maker loop
	void run();

	//Spread the zero button
	int spreadZero(LButton* zero_button);

	//Generate the bombs
	void generateGame();

	void start_game();

	void lose_game();

	void win_game();
};

#endif