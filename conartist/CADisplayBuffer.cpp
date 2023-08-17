#include "CADisplayBuffer.h"


#ifndef UTF_CPP_CPLUSPLUS
#define UTF_CPP_CPLUSPLUS 201103L
#endif
#include "utfcpp/source/utf8.h"

void ConArtist::CADisplayBuffer::putText(int x, int y, char32_t charToPut) {
	if (x >= 0 && x < this->width && y >= 0 && y < this->height) {
		this->textBuffer[y][x] = charToPut;
	}
}

void ConArtist::CADisplayBuffer::putColor(int x, int y, ConArtist::CAColor colorToPut) {
	if (x >= 0 && x < this->width && y >= 0 && y < this->height) {
		this->colorBuffer[y][x] = colorToPut;
	}
}

ConArtist::CADisplayBuffer::CADisplayBuffer(int width, int height) : width{ width }, height{ height } {
	this->textBuffer = std::vector<std::u32string>(height, std::u32string(width, U' '));
	this->colorBuffer = std::vector<std::vector<ConArtist::CAColor>>(height, std::vector<ConArtist::CAColor>(width, ConArtist::CAColor()));
}

void ConArtist::CADisplayBuffer::clearBuffers() {
	for (int y = 0; y < this->height; y++) {
		for (int x = 0; x < this->width; x++) {
			putText(x, y, U' ');
			putColor(x, y, ConArtist::CAColor(0,0,0));
		}
	}
}

void ConArtist::CADisplayBuffer::clearTextBuffer()
{
	for (int y = 0; y < this->height; y++) {
		for (int x = 0; x < this->width; x++) {
			putText(x, y, U' ');
		}
	}
}

void ConArtist::CADisplayBuffer::drawTextSprite(int x, int y, CAMonoSprite sprite) {
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

void ConArtist::CADisplayBuffer::drawTextSprite(Coords2D coordinates, CAMonoSprite sprite) {
	drawTextSprite(coordinates.x, coordinates.y, sprite);
}

void ConArtist::CADisplayBuffer::drawText(int x, int y, char32_t charToWrite) {
	if (x >= 0 && x < this->width && y >= 0 && y < this->height) {
		putText(x, y, charToWrite);
	}
}

void ConArtist::CADisplayBuffer::drawText(int x, int y, char charToWrite) {
	ConArtist::CADisplayBuffer::drawText(x, y, utf8::utf8to32(std::string(1, charToWrite))[0]);
}

void ConArtist::CADisplayBuffer::drawText(int x, int y, std::u32string stringToWrite) {
	int i = 0, x1 = x, y1 = y;
	while (i < stringToWrite.size() && x1 >= 0 && y1 >= 0 && x1 < this->width && y1 < this->height) {
		if (stringToWrite[i] == U'\n') {
			x1 = x;
			y1++;
		}
		else {
			putText(x1, y1, stringToWrite[i]);
			x1++;
		}
		i++;
	}
}

void ConArtist::CADisplayBuffer::drawText(int x, int y, std::string stringToWrite) {
	ConArtist::CADisplayBuffer::drawText(x, y, utf8::utf8to32(stringToWrite));
}

void ConArtist::CADisplayBuffer::drawText(Coords2D coordinates, std::string stringToWrite) {
	this->drawText(coordinates.x, coordinates.y, stringToWrite);
}

void ConArtist::CADisplayBuffer::fillText(int left, int top, int right, int bottom, char charToWrite) {
	ConArtist::CADisplayBuffer::fillText(left, top, right, bottom, utf8::utf8to32(std::string(1, charToWrite))[0]);
}

void ConArtist::CADisplayBuffer::fillText(int left, int top, int right, int bottom, char32_t charToWrite) {
	int x = left, y = top;
	for (int y = top; y <= bottom; y++) {
		for (int x = left; x <= right; x++) {
			putText(x, y, charToWrite);
		}
	}
}

void ConArtist::CADisplayBuffer::fillColor(int left, int top, int right, int bottom, ConArtist::CAColor colorToDraw) {
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

void ConArtist::CADisplayBuffer::fillColor(int x, int y, ConArtist::CAColor colorToDraw) {
	if (x >= 0 && x < this->width && y >= 0 && y < this->height) {
		putColor(x, y, colorToDraw);
	}
}