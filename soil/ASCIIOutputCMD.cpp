#include "ASCIIOutputCMD.h"

Soil::ASCIIOutputCMD::ASCIIOutputCMD() {

	// store reference to console buffer that launched the game
	this->originalConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// create console buffer for the game to run in
	this->gameConsoleHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(this->gameConsoleHandle);

	// disable the cursor visibility
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(this->gameConsoleHandle, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(this->gameConsoleHandle, &cursorInfo);

	// disable mouse input
	DWORD consoleMode;
	GetConsoleMode(this->gameConsoleHandle, &consoleMode);
	consoleMode ^= ENABLE_MOUSE_INPUT;
	SetConsoleMode(this->gameConsoleHandle, consoleMode);

	// set console buffer size
	/*CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
	GetConsoleScreenBufferInfo(hConsole, &consoleBufferInfo);
	consoleBufferInfo.srWindow.Top = 0;
	consoleBufferInfo.srWindow.Left = 0;
	consoleBufferInfo.srWindow.Right = 80;
	consoleBufferInfo.srWindow.Bottom = 25;
	SetConsoleWindowInfo(hConsole, TRUE, &(consoleBufferInfo.srWindow));*/
}

Soil::ASCIIOutputCMD::~ASCIIOutputCMD() {
	SetConsoleActiveScreenBuffer(this->originalConsoleHandle);
}

void Soil::ASCIIOutputCMD::pushOutput(ASCIIGraphics& asciiGraphics) {

	// check what size our window is
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
	GetConsoleScreenBufferInfo(this->gameConsoleHandle, &consoleScreenBufferInfo);

	if (
		consoleScreenBufferInfo.dwMaximumWindowSize.X != this->currentWindowSize.X || 
		consoleScreenBufferInfo.dwMaximumWindowSize.Y != this->currentWindowSize.Y
		) {

		// resize our buffer to fit the window size
		SetConsoleScreenBufferSize(this->gameConsoleHandle, consoleScreenBufferInfo.dwMaximumWindowSize);

		// clear the buffer of any leftover characters from the previous size
		DWORD garbage;
		FillConsoleOutputCharacter(
			this->gameConsoleHandle,
			' ',
			consoleScreenBufferInfo.dwMaximumWindowSize.X * consoleScreenBufferInfo.dwMaximumWindowSize.Y, { 0,0 }, &garbage
		);

		// store the size for comparison the next time we check this `if` statement
		this->currentWindowSize = consoleScreenBufferInfo.dwMaximumWindowSize;
	}


	// find the coordinates we need to draw our ASCIIGraphics at to be centered
	short xOrigin = std::max(0, (consoleScreenBufferInfo.dwMaximumWindowSize.X - asciiGraphics.width) / 2);
	short yOrigin = std::max(0, (consoleScreenBufferInfo.dwMaximumWindowSize.Y - asciiGraphics.height) / 2);

	DWORD dwBytesWritten;
	for (short y = 0; y < asciiGraphics.height; y++) {
		WriteConsoleOutputAttribute(
			this->gameConsoleHandle, 
			&asciiGraphics.attributeBuffer[y * asciiGraphics.width], 
			asciiGraphics.width, 
			{ xOrigin, yOrigin + y }, 
			&dwBytesWritten
		);
		
		WriteConsoleOutputCharacter(
			this->gameConsoleHandle, 
			asciiGraphics.textBuffer.c_str() + y * asciiGraphics.width, 
			asciiGraphics.width, 
			{ xOrigin, yOrigin + y }, 
			&dwBytesWritten
		);
	}
}
