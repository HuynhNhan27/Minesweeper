#ifndef LBUTTON_H
#define LBUTTON_H

#include "utils.h"

//The mouse button
class LButton
{
public:
	//Initializes internal variables
	LButton();

	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	////Shows button sprite
	//void render();

	//Gets button position
	SDL_Point getPosition();

	//Gets button sprite
	LButtonSprite getCurrentSprite();

private:
	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;
};

#endif