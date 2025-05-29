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
	LButton* gButtons = NULL;
public:
	Maker();
	~Maker();

	//Initialize the maker
	bool init();

	//Load media for the maker
	bool loadMedia();

	//Free resources and close SDL
	void close();
	
	//Renders the buttons
	void render(int i);

	//Run the maker loop
	void run();
};

#endif