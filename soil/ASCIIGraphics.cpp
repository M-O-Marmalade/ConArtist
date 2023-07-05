#include "ASCIIGraphics.h"

#include "utfcpp/source/utf8.h"

void Soil::ASCIIGraphics::putText(int x, int y, char32_t charToPut) {
	if (x >= 0 && x < this->width && y >= 0 && y < this->height) {
		this->textBuffer[y][x] = charToPut;
	}
}

void Soil::ASCIIGraphics::putColor(int x, int y, Soil::ASCIIColor colorToPut) {
	if (x >= 0 && x < this->width && y >= 0 && y < this->height) {
		this->colorBuffer[y][x] = colorToPut;
	}
}

Soil::ASCIIGraphics::ASCIIGraphics(int width, int height) : width{ width }, height{ height } {
	this->textBuffer = std::vector<std::u32string>(height, std::u32string(width, U' '));
	this->colorBuffer = std::vector<std::vector<Soil::ASCIIColor>>(height, std::vector<Soil::ASCIIColor>(width, Soil::ASCIIColor(0,0,0)));
}

void Soil::ASCIIGraphics::clearBuffers() {
	for (int y = 0; y < this->height; y++) {
		for (int x = 0; x < this->width; x++) {
			putText(x, y, U' ');
			putColor(x, y, Soil::ASCIIColor(0,0,0));
		}
	}
}

void Soil::ASCIIGraphics::drawTextSprite(int x, int y, ASCIISprite sprite) {
	int i = 0, x1 = x, y1 = y;
	while (i < sprite.text.size() && x1 >= 0 && y1 >= 0 && x1 < this->width && y1 < this->height) {
		if (sprite.text[i] == U'\n') {
			x1 = x;
			y1++;
		}
		else {
			putText(x1, y1, sprite.text[i]);
			putColor(x1, y1, sprite.color);

			x1++;
		}
		i++;
	}
}

void Soil::ASCIIGraphics::drawTextSprite(Coords2D coordinates, ASCIISprite sprite) {
	drawTextSprite(coordinates.x, coordinates.y, sprite);
}

void Soil::ASCIIGraphics::drawText(int x, int y, char32_t charToWrite) {
	if (x >= 0 && x < this->width && y >= 0 && y < this->height) {
		putText(x, y, charToWrite);
	}
}

void Soil::ASCIIGraphics::drawText(int x, int y, char charToWrite) {
	Soil::ASCIIGraphics::drawText(x, y, utf8::utf8to32(std::string(1, charToWrite))[0]);
}

void Soil::ASCIIGraphics::drawText(int x, int y, std::string stringToWrite) {
	std::u32string stringToWrite32 = utf8::utf8to32(stringToWrite);
	int i = 0, x1 = x, y1 = y;
	while (i < stringToWrite32.size() && x1 >= 0 && y1 >= 0 && x1 < this->width && y1 < this->height) {
		if (stringToWrite32[i] == U'\n') {
			x1 = x;
			y1++;
		}
		else {
			putText(x1, y1, stringToWrite32[i]);
			x1++;
		}
		i++;
	}
}

void Soil::ASCIIGraphics::drawText(Coords2D coordinates, std::string stringToWrite) {
	this->drawText(coordinates.x, coordinates.y, stringToWrite);
}

void Soil::ASCIIGraphics::fillText(int left, int top, int right, int bottom, char charToWrite) {
	Soil::ASCIIGraphics::fillText(left, top, right, bottom, utf8::utf8to32(std::string(1, charToWrite))[0]);
}

void Soil::ASCIIGraphics::fillText(int left, int top, int right, int bottom, char32_t charToWrite) {
	int x = left, y = top;
	for (int y = top; y <= bottom; y++) {
		for (int x = left; x <= right; x++) {
			putText(x, y, charToWrite);
		}
	}
}

void Soil::ASCIIGraphics::fillColor(int left, int top, int right, int bottom, Soil::ASCIIColor colorToDraw) {
	int x = std::max(left, 0), y = std::max(top, 0);
	while (y <= bottom && y < this->height) {
		putColor(x, y, colorToDraw);
		if (x < right && x < this->width - 1) {
			x++;
		}
		else {
			x = left;
			y++;
		}
	}
}

void Soil::ASCIIGraphics::fillColor(int x, int y, Soil::ASCIIColor colorToDraw) {
	if (x >= 0 && x < this->width && y >= 0 && y < this->height) {
		putColor(x, y, colorToDraw);
	}
}