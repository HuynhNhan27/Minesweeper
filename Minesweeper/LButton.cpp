#include "LButton.h"
//#include "LTexture.h"

LButton::LButton() {
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_DEFAULT;
	pressed = false;
	// State is set random by maker
}

void LButton::setPosition(int x, int y) {
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::setCurrentSprite(LButtonSprite newSprite) {
	mCurrentSprite = newSprite;
}

void LButton::setState(int newState) {
	state = newState;
}

void LButton::setPressed(bool newPressed) {
	pressed = newPressed;
}

void LButton::handleEvent(SDL_Event* e) {
	//Only called when mouse is pressed
	if (e->type == SDL_MOUSEBUTTONDOWN) {
		// Ignore if the button is pressed before
		if (pressed) return;

		// If left mouse button is pressed
		if (e->button.button == SDL_BUTTON_LEFT) {
			if (mCurrentSprite == BUTTON_SPRITE_DEFAULT) {
				if (state == -1) {
					mCurrentSprite = BUTTON_SPRITE_EXPLOSION;
				}
				else {
					mCurrentSprite = (LButtonSprite)state;
				}
			}
			else {
				return; // Ignore if the button is flagged 
			}
		}
		// If right mouse button is pressed
		else if (e->button.button == SDL_BUTTON_RIGHT) {
			if (mCurrentSprite == BUTTON_SPRITE_DEFAULT) {
				mCurrentSprite = BUTTON_SPRITE_FLAG;
			}
			else if (mCurrentSprite == BUTTON_SPRITE_FLAG) {
				mCurrentSprite = BUTTON_SPRITE_DEFAULT;
			}
		}
	}
}

SDL_Point LButton::getPosition() {
	return mPosition;
}

LButtonSprite LButton::getCurrentSprite() {
	return mCurrentSprite;
}
