#include "maker.h"
#include <random>
#include <queue>

Maker::Maker(int width, int height, int bombs) {
	mWindow.GAME_WIDTH = width;
	mWindow.GAME_HEIGHT = height;
	mWindow.BOMBS = bombs;
	mWindow.TOTAL_BUTTONS = width * height;
	mWindow.SCREEN_WIDTH = width * CELL_SIZE + 24;
	mWindow.SCREEN_HEIGHT = height * CELL_SIZE + 68;
}
Maker::~Maker() { }

bool Maker::init() {
	//Initialization flag
	bool success = true;

	gSpriteClips = new SDL_Rect[BUTTON_SPRITE_TOTAL];
	background = new LButton[13];

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
		//Set cells sprite clips: 0-17
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 9; ++j) {
				gSpriteClips[i * 9 + j].x = j * 16;
				gSpriteClips[i * 9 + j].y = i * 16;
				gSpriteClips[i * 9 + j].w = BUTTON_WIDTH;
				gSpriteClips[i * 9 + j].h = BUTTON_HEIGHT;
			}
		}

		//Set timer sprite clips: 20-30
		for (int i = 0; i <= 10; ++i) {
			gSpriteClips[i + 20].x = i * 12;
			gSpriteClips[i + 20].y = 33;
			gSpriteClips[i + 20].w = TIMER_WIDTH;
			gSpriteClips[i + 20].h = TIMER_HEIGHT;
		}

		//Set face sprite clips: 31-35
		for (int i = 0; i < 5; ++i) {
			gSpriteClips[i + 31].x = i * 27;
			gSpriteClips[i + 31].y = 55;
			gSpriteClips[i + 31].w = FACE_WIDTH;
			gSpriteClips[i + 31].h = FACE_HEIGHT;
		}

		//Set corner sprite clips: 40-45
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 2; ++j) {
				gSpriteClips[i * 2 + j + 40].x = j * 15;
				gSpriteClips[i * 2 + j + 40].y = i * 14 + 82;
				gSpriteClips[i * 2 + j + 40].w = CORNER_WIDTH;
				gSpriteClips[i * 2 + j + 40].h = CORNER_HEIGHT;
			}
		}

		//Set border sprite clips: 46-52
		for (int i = 0; i < 3; ++i) { //46-48: use for width
			gSpriteClips[i + 46].x = 13;
			gSpriteClips[i + 46].y = i * 14 + 82;
			gSpriteClips[i + 46].w = BORDER_WIDTH;
			gSpriteClips[i + 46].h = BORDER_HEIGHT;
		}
		for (int i = 0; i < 2; ++i) {
			for (int j = 0; j < 2; ++j) {
				gSpriteClips[i * 2 + j + 49].x = j * 15;
				gSpriteClips[i * 2 + j + 49].y = i * 14 + 94;
				gSpriteClips[i * 2 + j + 49].w = BORDER_HEIGHT;
				gSpriteClips[i * 2 + j + 49].h = BORDER_WIDTH + 1;
			}
		}

		//Set Timer Window sprite clips: 53
		gSpriteClips[53].x = 28;
		gSpriteClips[53].y = 82;
		gSpriteClips[53].w = TIMER_WINDOW_WIDTH;
		gSpriteClips[53].h = TIMER_WINDOW_HEIGHT;
	}

	// Set face position
	background[12].setPosition(mWindow.SCREEN_WIDTH / 2 - 13, 16);

	return success;
}

bool Maker::renderBackground() {
	// Render the top border
	for (int i = 0; i < mWindow.GAME_WIDTH * 16; ++i) {
		background[6].setPosition(12 + i, 0);
		gButtonSpriteSheetTexture.render(background[6].getPosition(), &gSpriteClips[46]);
	}

	for (int i = 0; i < mWindow.GAME_WIDTH * 16; ++i) {
		background[6].setPosition(12 + i, 46);
		gButtonSpriteSheetTexture.render(background[6].getPosition(), &gSpriteClips[47]);
	}

	for (int i = 0; i < mWindow.GAME_WIDTH * 16; ++i) {
		background[6].setPosition(12 + i, mWindow.GAME_HEIGHT * 16 + 57);
		gButtonSpriteSheetTexture.render(background[6].getPosition(), &gSpriteClips[48]);
	}

	// Render the left right border
	for (int i = 0; i < 35; ++i) {
		background[6].setPosition(0, i + 11);
		gButtonSpriteSheetTexture.render(background[6].getPosition(), &gSpriteClips[49]);
		background[6].setPosition(mWindow.GAME_WIDTH * 16 + 12, i + 11);
		gButtonSpriteSheetTexture.render(background[6].getPosition(), &gSpriteClips[50]);
	}

	for (int i = 0; i < mWindow.GAME_HEIGHT * 16; ++i) {
		background[6].setPosition(0, 57 + i);
		gButtonSpriteSheetTexture.render(background[6].getPosition(), &gSpriteClips[51]);
		background[6].setPosition(mWindow.GAME_WIDTH * 16 + 12, 57 + i);
		gButtonSpriteSheetTexture.render(background[6].getPosition(), &gSpriteClips[52]);
	}

	// Render the corners
	background[0].setPosition(0, 0); // Top-left corner
	background[1].setPosition(mWindow.GAME_WIDTH * 16 + 12, 0); // Top-right corner
	background[2].setPosition(0, 46); // Middle-left corner
	background[3].setPosition(mWindow.GAME_WIDTH * 16 + 12, 46); // Middle-right corner
	background[4].setPosition(0, mWindow.GAME_HEIGHT * 16 + 57); // Bottom-left corner
	background[5].setPosition(mWindow.GAME_WIDTH * 16 + 12, mWindow.GAME_HEIGHT * 16 + 57); // Bottom-right corner

	for (int i = 0; i < 6; ++i) {
		gButtonSpriteSheetTexture.render(background[i].getPosition(), &gSpriteClips[40 + i]);
	}

	// Render the face button
	gButtonSpriteSheetTexture.render(background[12].getPosition(), &gSpriteClips[background[12].getCurrentSprite()]);

	return true;
}

void Maker::close() {
	//Free loaded images
	gButtonSpriteSheetTexture.free();

	//Free allocated memory for buttons and sprite clips
	for (int i = 0; i < mWindow.GAME_HEIGHT; ++i) {
		delete[] gButtons[i];
	}
	delete[] gButtons;
	delete[] gSpriteClips;
	delete[] background;

	//Destroy window	
	SDL_DestroyRenderer(mWindow.gRenderer);
	SDL_DestroyWindow(mWindow.gWindow);
	mWindow.gWindow = NULL;
	mWindow.gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


void Maker::renderAll() {
	//Clear screen
	SDL_SetRenderDrawColor(mWindow.gRenderer, 0x8C, 0x8C, 0x8C, 0xFF); // For a gray background
	SDL_RenderClear(mWindow.gRenderer);

	//Render buttons
	for (int i = 0; i < mWindow.GAME_HEIGHT; ++i) {
		for (int j = 0; j < mWindow.GAME_WIDTH; ++j) {
			gButtonSpriteSheetTexture.render(gButtons[i][j].getPosition(), &gSpriteClips[gButtons[i][j].getCurrentSprite()]);
		}
	}
	renderBackground();

	//Update screen
	SDL_RenderPresent(mWindow.gRenderer);
}

void Maker::run() {
	//Start up SDL and create window
	if (!init()) {
		printf("Failed to initialize!\n");
		return;
	}

	//Load media
	if (!loadMedia()) {
		printf("Failed to load media!\n");
		return;
	}

	// Generate the game
	start_game();

	//Main loop flag
	bool quit = false;
	int event = 0;
	//Event handler
	SDL_Event e;

	//While application is running
	while (!quit) {
		//Handle events on queue
		SDL_WaitEvent(&e);

		switch (e.type) {
		case SDL_QUIT:
			quit = true;
			break;

		case SDL_MOUSEBUTTONDOWN:
			int x, y;
			SDL_GetMouseState(&x, &y);
			if (x >= background[12].getPosition().x && x < background[12].getPosition().x + FACE_WIDTH &&
				y >= background[12].getPosition().y && y < background[12].getPosition().y + FACE_HEIGHT) {
				start_game(); // Restart the game if face button is clicked
				event = 0;
			}
			else if (event != -1) {
				x = (x - 12) / CELL_SIZE;
				y = (y - 57) / CELL_SIZE;
				if (x >= 0 && y >= 0 && x < mWindow.GAME_WIDTH && y < mWindow.GAME_HEIGHT) {
					event = gButtons[y][x].handleEvent(&e);
					if (event == 0) --win_condition;
					if (event == 1) win_condition = win_condition - spreadZero(&gButtons[y][x]) - 1;
				}

				if (win_condition <= 0) win_game();
				// Render the updated state of the game
				renderAll();

				if (event == -1) {
					// If a bomb was clicked
					lose_game();
				}
			}
			break;
		}
	}

	//Free resources and close SDL
	close();

	return;
}

int Maker::spreadZero(LButton* zero_button) {
	queue<LButton*> buttons;
	int plusX[] = { -1, 0, 1, -1, 1, -1, 0, 1 }, plusY[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int res = 0;
	buttons.push(zero_button);
	while (!buttons.empty()) {
		LButton* curr = buttons.front();
		buttons.pop();
		for (int i = 0; i < 8; ++i) {
			int x = (curr->getPosition().x - 12) / CELL_SIZE + plusX[i];
			int y = (curr->getPosition().y - 57) / CELL_SIZE + plusY[i];
			if (x >= 0 && y >= 0 && x < mWindow.GAME_WIDTH && y < mWindow.GAME_HEIGHT) {
				LButton* b = &gButtons[y][x];
				if (b->getCurrentSprite() == BUTTON_SPRITE_DEFAULT && b->getState() != -1) {
					if (b->getState() == 0) {
						buttons.push(b);
					}
					b->setCurrentSprite((LButtonSprite)b->getState());
					++res;
				}
			}
		}
	}

	return res;
}

void Maker::generateGame() {
	if (gButtons != NULL) {
		for (int i = 0; i < mWindow.GAME_HEIGHT; ++i) {
			delete[] gButtons[i];
		}
		delete[] gButtons;
	}
	gButtons = new LButton * [mWindow.GAME_HEIGHT];
	for (int i = 0; i < mWindow.GAME_HEIGHT; ++i) {
		gButtons[i] = new LButton[mWindow.GAME_WIDTH];
	}

	win_condition = mWindow.TOTAL_BUTTONS - mWindow.BOMBS;
	// Reset the face button to default state
	background[12].setCurrentSprite(BUTTON_SPRITE_FACE);

	// Initialize positions of buttons
	for (int i = 0; i < mWindow.GAME_HEIGHT; ++i) {
		for (int j = 0; j < mWindow.GAME_WIDTH; ++j) {
			gButtons[i][j].setPosition(j * 16 + 12, i * 16 + 57);
		}
	}

	// Initialize the number or bomb
	int* arr = new int[mWindow.TOTAL_BUTTONS], * indices = new int[mWindow.TOTAL_BUTTONS];
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
		gButtons[i / mWindow.GAME_WIDTH][i % mWindow.GAME_WIDTH].setState(arr[i]);
	}

	delete[] indices;
	delete[] arr;
}

void Maker::start_game() {
	// Reset the game
	generateGame();
	renderAll();
}

void Maker::lose_game() {
	// Show all bombs
	for (int i = 0; i < mWindow.GAME_HEIGHT; ++i) {
		for (int j = 0; j < mWindow.GAME_WIDTH; ++j) {
			if (gButtons[i][j].getState() == -1) {
				gButtons[i][j].setCurrentSprite(BUTTON_SPRITE_EXPLOSION);
			}
		}
	}
	background[12].setCurrentSprite(BUTTON_SPRITE_LOSE_FACE);
	renderAll();
}

void Maker::win_game() {
	// Show all bombs and mark the rest as safe
	for (int i = 0; i < mWindow.GAME_HEIGHT; ++i) {
		for (int j = 0; j < mWindow.GAME_WIDTH; ++j) {
			if (gButtons[i][j].getState() == -1) {
				gButtons[i][j].setCurrentSprite(BUTTON_SPRITE_FLAG);
			}
		}
	}
	background[12].setCurrentSprite(BUTTON_SPRITE_WIN_FACE);
}