#include "ASCIIGraphics.h"

ASCIIGraphics::ASCIIGraphics(int width, int height) : width{ width }, height{ height } {
	this->textBuffer = std::string(width * height, ' ');
	this->attributeBuffer = std::vector<WORD>(width * height, FOREGROUND_RED);
}

void ASCIIGraphics::drawTextSprite(int x, int y, ASCIISprite sprite) {
	int i = 0, x1 = x, y1 = y;
	while (i < sprite.text.size() && x1 >= 0 && y1 >= 0 && x1 < this->width && y1 < this->height) {
		if (sprite.text[i] == '\n') {
			x1 = x;
			y1++;
		}
		else {
			this->textBuffer[x1 + y1 * this->width] = sprite.text[i];
			this->attributeBuffer[x1 + y1 * this->width] = sprite.color;
			x1++;
		}
		i++;
	}
}

void ASCIIGraphics::drawTextSprite(Coords2D coordinates, ASCIISprite sprite) {
	drawTextSprite(coordinates.x, coordinates.y, sprite);
}

void ASCIIGraphics::drawText(int x, int y, std::string stringToWrite) {
	int i = 0, x1 = x, y1 = y;
	while (i < stringToWrite.size() && x1 >= 0 && y1 >= 0 && x1 < this->width && y1 < this->height) {
		if (stringToWrite[i] == '\n') {
			x1 = x;
			y1++;
		}
		else {
			this->textBuffer[x1 + y1 * this->width] = stringToWrite[i];
			x1++;
		}
		i++;
	}
}

void ASCIIGraphics::drawText(Coords2D coordinates, std::string stringToWrite) {
	this->drawText(coordinates.x, coordinates.y, stringToWrite);
}

void ASCIIGraphics::fillText(int left, int top, int right, int bottom, wchar_t charToWrite) {
	int x = left, y = top;
	for (int y = top; y <= bottom; y++) {
		for (int x = left; x <= right; x++) {
			this->textBuffer[x + y * this->width] = charToWrite;
		}
	}
}

void ASCIIGraphics::fillColor(WORD colorToDraw, int left, int top, int right, int bottom) {
	int x = std::max(left, 0), y = std::max(top, 0);
	while (y <= bottom && y < this->height) {
		this->attributeBuffer[x + y * this->width] = colorToDraw; // color foreground & background
		if (x < right && x < this->width - 1) {
			x++;
		}
		else {
			x = left;
			y++;
		}
	}
}

void ASCIIGraphics::fillColor(WORD colorToDraw, int x, int y) {
		this->attributeBuffer[x + y * this->width] = colorToDraw; // color foreground & background
}
