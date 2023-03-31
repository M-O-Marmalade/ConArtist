#pragma once
#include <string>
#include <vector>
#define NOMINMAX
#include <Windows.h>

#include "ASCIISprite.h"
#include "Coords2D.h"

class ASCIIGraphics {
public:
	int width;
	int height;
	std::string textBuffer;
	std::vector<WORD> attributeBuffer;

	ASCIIGraphics(int width, int height);

	void drawTextSprite(int x, int y, ASCIISprite sprite);
	void drawTextSprite(Coords2D coordinates, ASCIISprite sprite);
	void drawText(int x, int y, std::string stringToWrite);
	void drawText(Coords2D coordinates, std::string stringToWrite);
	void fillText(int left, int top, int right, int bottom, wchar_t charToWrite);
	void fillColor(WORD colorToDraw, int left, int top, int right, int bottom);
	void fillColor(WORD colorToDraw, int x, int y);
};

