#ifndef LBUTTON_H
#define LBUTTON_H

#include "utils.h"

//The mouse button
class LButton
{
public:
	//Initializes internal variables
	LButton();

	//Sets position
	void setPosition(int x, int y);

	//Sets Current Sprite
	void setCurrentSprite(LButtonSprite newSprite);

	void setState(int newState);

	void setPressed(bool newPressed);

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	////Shows button sprite
	//void render();

	//Gets button position
	SDL_Point getPosition();

	//Gets button sprite
	LButtonSprite getCurrentSprite();

private:
	//Position
	SDL_Point mPosition;

	//Currently sprite
	LButtonSprite mCurrentSprite;

	//Real state: -1 = bomb, 0-8 = number
	int state;

	//Is the button pressed
	bool pressed;
};

#endif