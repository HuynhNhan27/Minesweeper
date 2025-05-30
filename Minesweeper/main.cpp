#include "maker.h"
#include <iostream>

int main(int argc, char* args[])
{
	cout << "Minesweeper Game" << endl;
	cout << "Please choose a difficulty level:" << endl;
	cout << "1. Beginner (9x9, 10 mines)" << endl;
	cout << "2. Intermediate (16x16, 40 mines)" << endl;
	cout << "3. Expert (30x16, 99 mines)" << endl;
	cout << "4. Custom (Specify size and number of mines)" << endl;
	cout << "Choose difficulty: 1-4: ";
	int mode;
	cin >> mode;
	if (mode == 1) {
		Maker maker(9, 9, 10);
		maker.run();
	}
	else if (mode == 2) {
		Maker maker(16, 16, 40);
		maker.run();
	}
	else if (mode == 3) {
		Maker maker(30, 16, 99);
		maker.run();
	}
	else if (mode == 4) {
		int width, height, mines;
		cout << "Enter width: ";
		cin >> width;
		cout << "Enter height: ";
		cin >> height;
		cout << "Enter number of mines: ";
		cin >> mines;
		if (width <= 0 || height <= 0 || mines <= 0 || mines >= width * height) {
			cout << "Invalid input. Please try again." << endl;
			return 1;
		}

		Maker maker(width, height, mines);
		maker.run();
	}
	else {
		cout << "Invalid choice. Exiting." << endl;
		return 1;
	}

	return 0;
}