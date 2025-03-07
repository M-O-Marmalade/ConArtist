// the CADisplayBuffer class is a "screen" that stores buffers of Unicode text and ANSI color data.
// This is the class that you interface with when drawing graphics.
// When you are ready to display the graphics data this class contains to a terminal window, you pass this class/object to an CADisplayOutput class/object, which pushes data from this class to the OS's terminal display.

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#define NOMINMAX
#include <Windows.h>

#include "CAColor.h"
#include "CAMonoSprite.h"
#include "Coords2D.h"

namespace ConArtist {
	class CADisplayBuffer {
	private:
		void putText(int x, int y, char32_t charToPut);
		void putColor(int x, int y, ConArtist::CAColor colorToPut);

	public:
		int width;
		int height;

		// buffers are indexed [y,x], origin ([0,0]) is the top-left cell of the console
		std::vector<std::u32string> textBuffer;
		std::vector<std::vector<ConArtist::CAColor>> colorBuffer;

		CADisplayBuffer(int width, int height);

		void clearBuffers();
		void clearTextBuffer();
		void drawTextSprite(int x, int y, CAMonoSprite sprite);
		void drawTextSprite(Coords2D coordinates, CAMonoSprite sprite);
		void drawText(int x, int y, char32_t charToWrite);
		void drawText(int x, int y, char charToWrite);
		void drawText(int x, int y, std::u32string stringToWrite);
		void drawText(int x, int y, std::string stringToWrite);
		void drawText(Coords2D coordinates, std::string stringToWrite);
		void fillText(int left, int top, int right, int bottom, char charToWrite);
		void fillText(int left, int top, int right, int bottom, char32_t charToWrite);
		void fillColor(int left, int top, int right, int bottom, ConArtist::CAColor colorToDraw);
		void fillColor(int x, int y, ConArtist::CAColor colorToDraw);
	};
}