#include "maker.h"
#include <random>

Maker::Maker() { }
Maker::~Maker() { }

bool Maker::init() {
	//Initialization flag
	bool success = true;

	gSpriteClips = new SDL_Rect[BUTTON_SPRITE_TOTAL];
	gButtons = new LButton[mWindow.TOTAL_BUTTONS];

	///////////////////////////////////////////////////////////
	// Set random bombs positions
	int* arr = new int[mWindow.TOTAL_BUTTONS], *indices = new int[mWindow.TOTAL_BUTTONS];
	for (int i = 0; i < mWindow.TOTAL_BUTTONS; ++i) {
		arr[i] = 0;
		indices[i] = i;
	}

	random_device rd;
	mt19937 gen(rd());
	shuffle(indices, indices + mWindow.TOTAL_BUTTONS, gen);

	for (int i = 0; i < mWindow.BOMBS; ++i) {
		arr[indices[i]] = -1; // Set bomb
	}

	for (int i = 0; i < mWindow.TOTAL_BUTTONS; ++i) {
		if (arr[i] == -1) continue;
		int x = i % mWindow.GAME_WIDTH;
		int y = i / mWindow.GAME_WIDTH;

		bool up = y - 1 >= 0, down = y + 1 < mWindow.GAME_HEIGHT;
		bool left = x - 1 >= 0, right = x + 1 < mWindow.GAME_WIDTH;

		if (up) {
			if (arr[i - mWindow.GAME_WIDTH] == -1) arr[i]++;
			if (left && arr[i - mWindow.GAME_WIDTH - 1] == -1) arr[i]++;
			if (right && arr[i - mWindow.GAME_WIDTH + 1] == -1) arr[i]++;
		}
		if (down) {
			if (arr[i + mWindow.GAME_WIDTH] == -1) arr[i]++;
			if (left && arr[i + mWindow.GAME_WIDTH - 1] == -1) arr[i]++;
			if (right && arr[i + mWindow.GAME_WIDTH + 1] == -1) arr[i]++;
		}
		if (left && arr[i - 1] == -1) arr[i]++;
		if (right && arr[i + 1] == -1) arr[i]++;
	}

	for (int i = 0; i < mWindow.TOTAL_BUTTONS; ++i) {
		gButtons[i].setState(arr[i]);
	}

	delete[] indices;
	delete[] arr;
	////////////////////////////////////////////////////////////

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
		for (int i = 0; i < 2; ++i) { // BUTTON_SPRITE_TOTAL
			for (int j = 0; j < 9; ++j) {
				gSpriteClips[i * 9 + j].x = j * 16;
				gSpriteClips[i * 9 + j].y = i * 16;
				gSpriteClips[i * 9 + j].w = mWindow.BUTTON_WIDTH;
				gSpriteClips[i * 9 + j].h = mWindow.BUTTON_HEIGHT;
			}
		}

		for (int i = 0; i < mWindow.GAME_HEIGHT; ++i) {
			for (int j = 0; j < mWindow.GAME_WIDTH; ++j) {
				gButtons[i * mWindow.GAME_HEIGHT + j].setPosition(j * 16 + 1, i * 16 + 1);
			}
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
					switch (e.type) {
					case SDL_QUIT:
						quit = true;
						break;
					case SDL_MOUSEBUTTONDOWN:
						int x, y;
						SDL_GetMouseState(&x, &y);
						x /= CELL_SIZE;
						y /= CELL_SIZE;

						if (x >= 0 && y >= 0 && x < mWindow.GAME_WIDTH && y < mWindow.GAME_HEIGHT) {
							gButtons[y * mWindow.GAME_HEIGHT + x].handleEvent(&e);
						}
						break;
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor(mWindow.gRenderer, 0x8C, 0x8C, 0x8C, 0xFF); // For a gray background
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