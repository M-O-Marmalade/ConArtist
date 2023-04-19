#pragma once
#include <cstdint>
#include <string>
#include <vector>
#define NOMINMAX
#include <Windows.h>

#include "ASCIIColor.h"
#include "ASCIISprite.h"
#include "Coords2D.h"

namespace Soil {
	class ASCIIGraphics {
	private:
		void putText(int x, int y, char32_t charToPut);
		void putColor(int x, int y, Soil::ASCIIColor colorToPut);
	public:
		int width;
		int height;

		// buffers are indexed [y,x], origin ([0,0]) is the top-left cell of the console
		std::vector<std::u32string> textBuffer;
		std::vector<std::vector<bool>> changedTextCells;
		std::vector<int> changedTextRows;
		std::vector<int> changedTextColumns;

		std::vector<std::vector<Soil::ASCIIColor>> colorBuffer;
		std::vector<std::vector<bool>> changedColorCells;
		std::vector<int> changedColorRows;
		std::vector<int> changedColorColumns;

		ASCIIGraphics(int width, int height);

		void clearScreen();
		void resetTextObservers(bool val);
		void resetColorObservers(bool val);
		void drawTextSprite(int x, int y, ASCIISprite sprite);
		void drawTextSprite(Coords2D coordinates, ASCIISprite sprite);
		void drawText(int x, int y, char32_t charToWrite);
		void drawText(int x, int y, char charToWrite);
		void drawText(int x, int y, std::string stringToWrite);
		void drawText(Coords2D coordinates, std::string stringToWrite);
		void fillText(int left, int top, int right, int bottom, char charToWrite);
		void fillText(int left, int top, int right, int bottom, char32_t charToWrite);
		void fillColor(int left, int top, int right, int bottom, Soil::ASCIIColor colorToDraw);
		void fillColor(int x, int y, Soil::ASCIIColor colorToDraw);
	};
}