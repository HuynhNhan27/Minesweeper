#include "maker.h"

Maker::Maker() { }
Maker::~Maker() { }

bool Maker::init() {
	//Initialization flag
	bool success = true;

	gSpriteClips = new SDL_Rect[BUTTON_SPRITE_TOTAL];
	gButtons = new LButton[mWindow.TOTAL_BUTTONS];

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else {
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		mWindow.gWindow = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, mWindow.SCREEN_WIDTH, mWindow.SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (mWindow.gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			//Create vsynced renderer for window
			mWindow.gRenderer = SDL_CreateRenderer(mWindow.gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (mWindow.gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				//Initialize renderer color
				SDL_SetRenderDrawColor(mWindow.gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool Maker::loadMedia() {
	//Loading success flag
	bool success = true;

	//Load sprites
	if (!gButtonSpriteSheetTexture.loadFromFile("skin.bmp")) {
		printf("Failed to load skin texture!\n");
		success = false;
	}
	else {
		//Set sprites
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i) {
			gSpriteClips[i].x = i * 12;
			gSpriteClips[i].y = 0;
			gSpriteClips[i].w = mWindow.BUTTON_WIDTH;
			gSpriteClips[i].h = mWindow.BUTTON_HEIGHT;
		}

		//Set buttons in corners
		//gButtons[0].setPosition(0, 0);
		//gButtons[1].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, 0);
		//gButtons[2].setPosition(0, SCREEN_HEIGHT - BUTTON_HEIGHT);
		//gButtons[3].setPosition(SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i) {
			gButtons[i].setPosition(0, i * 12);
		}
	}

	return success;
}

void Maker::close() {
	//Free loaded images
	gButtonSpriteSheetTexture.free();

	//Free allocated memory for buttons and sprite clips
	delete[] gButtons;
	delete[] gSpriteClips;

	//Destroy window	
	SDL_DestroyRenderer(mWindow.gRenderer);
	SDL_DestroyWindow(mWindow.gWindow);
	mWindow.gWindow = NULL;
	mWindow.gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void Maker::render(int i) {
	//Show current button sprite
	gButtonSpriteSheetTexture.render(gButtons[i].getPosition(), &gSpriteClips[gButtons[i].getCurrentSprite()]);
}

void Maker::run() {
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					//Handle button events
					for (int i = 0; i < mWindow.TOTAL_BUTTONS; ++i)
					{
						gButtons[i].handleEvent(&e);
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(mWindow.gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(mWindow.gRenderer);

				//Render buttons
				for (int i = 0; i < mWindow.TOTAL_BUTTONS; ++i)
				{
					render(i);
				}

				//Update screen
				SDL_RenderPresent(mWindow.gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return;
}