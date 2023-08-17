#include "CADisplayOutputCMD.h"

#include <algorithm>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>

#ifndef UTF_CPP_CPLUSPLUS
#define UTF_CPP_CPLUSPLUS 201103L
#endif
#include "utfcpp/source/utf8.h"

#define ESC L"\x1b"
#define CSI L"\x1b["

void ConArtist::CADisplayOutputCMD::resizeBuffer(COORD* newDimensions) {
	
	// resize our console buffer to fit the window size
	SetConsoleScreenBufferSize(this->consoleOutputHandle, *newDimensions);

	// clear the buffer of any leftover characters from the previous size
	DWORD dwBytesWritten;
	FillConsoleOutputCharacter(this->consoleOutputHandle,
		' ',
		(*newDimensions).X * (*newDimensions).Y,
		{ 0,0 },
		&dwBytesWritten);

	FillConsoleOutputAttribute(this->consoleOutputHandle,
		(WORD)0,
		(*newDimensions).X * (*newDimensions).Y,
		{ 0,0 },
		&dwBytesWritten);

	// store the size for comparison the next time we check this `if` statement
	this->currentWindowSize = *newDimensions;

	// redraw the entire screen
	this->fullRedraw = true;

	// resize our lazy rendering buffers
	this->textBuffer = std::vector<std::u32string>((*newDimensions).Y, std::u32string((*newDimensions).X, U' '));
	this->colorBuffer = std::vector<std::vector<ConArtist::CAColor>>((*newDimensions).X, std::vector<ConArtist::CAColor>((*newDimensions).X, {0}));
}

bool ConArtist::CADisplayOutputCMD::cellNeedsUpdate(int x, int y, CADisplayBuffer& asciiGraphics, ConArtist::ANSIColorDepth currentColorDepth, int outputX, int outputY) {
	
	// if we're out of bounds, return false
	if (outputY >= this->textBuffer.size()
	 || outputY >= this->colorBuffer.size()
	 || outputY < 0
	 || outputX >= this->textBuffer[0].size()
	 || outputX >= this->colorBuffer[0].size()
	 || outputX < 0)
	{
		return false;
	}

	// if the previously drawn text doesn't match the text we're drawing, this cell needs to be updated
	if (asciiGraphics.textBuffer[y][x] != this->textBuffer[outputY][outputX]) {
		return true;
	}

	// if the previously drawn color depth doesn't match the one we're drawing, this cell needs to be updated
	if (currentColorDepth != this->colorBuffer[outputY][outputX].preferredColorDepth) {
		return true;
	}

	// if we're drawing 4bit color and the previously drawn color doesn't match the one we're considering drawing, it needs to be updated
	if (currentColorDepth == ANSI_4BIT_COLOR_DEPTH
	    && (   asciiGraphics.colorBuffer[y][x].ansi4BitColorFG != this->colorBuffer[outputY][outputX].ansi4BitColorFG
	        || asciiGraphics.colorBuffer[y][x].ansi4BitColorBG != this->colorBuffer[outputY][outputX].ansi4BitColorBG))
	{
		return true;
	}

	// if we're drawing 8bit color and the previously drawn color doesn't match the one we're considering drawing, it needs to be updated
	if (currentColorDepth == ANSI_8BIT_COLOR_DEPTH
	    && (   asciiGraphics.colorBuffer[y][x].ansi8BitColorFG != this->colorBuffer[outputY][outputX].ansi8BitColorFG
	        || asciiGraphics.colorBuffer[y][x].ansi8BitColorBG != this->colorBuffer[outputY][outputX].ansi8BitColorBG))
	{
		return true;
	}

	// if we're drawing 24bit color and the previously drawn color doesn't match the one we're considering drawing, it needs to be updated
	if (currentColorDepth == ANSI_24BIT_COLOR_DEPTH
	    && (   asciiGraphics.colorBuffer[y][x].ansi24BitTruecolorFG != this->colorBuffer[outputY][outputX].ansi24BitTruecolorFG
	        || asciiGraphics.colorBuffer[y][x].ansi24BitTruecolorBG != this->colorBuffer[outputY][outputX].ansi24BitTruecolorBG))
	{
		return true;
	}

	// if all else passes, then we just need to check if we're doing a full redraw
	return this->fullRedraw;
}

void ConArtist::CADisplayOutputCMD::storeCell(int x, int y, CADisplayBuffer& asciiGraphics, ConArtist::ANSIColorDepth currentColorDepth, int outputX, int outputY) {
	this->textBuffer[outputY][outputX] = asciiGraphics.textBuffer[y][x];
	this->colorBuffer[outputY][outputX].preferredColorDepth = currentColorDepth;
	if (currentColorDepth == ANSI_4BIT_COLOR_DEPTH) {
		this->colorBuffer[outputY][outputX].ansi4BitColorFG = asciiGraphics.colorBuffer[y][x].ansi4BitColorFG;
		this->colorBuffer[outputY][outputX].ansi4BitColorBG = asciiGraphics.colorBuffer[y][x].ansi4BitColorBG;

	}
	else if (currentColorDepth == ANSI_8BIT_COLOR_DEPTH) {
		this->colorBuffer[outputY][outputX].ansi8BitColorFG = asciiGraphics.colorBuffer[y][x].ansi8BitColorFG;
		this->colorBuffer[outputY][outputX].ansi8BitColorBG = asciiGraphics.colorBuffer[y][x].ansi8BitColorBG;
	}
	else if (currentColorDepth == ANSI_24BIT_COLOR_DEPTH) {
		this->colorBuffer[outputY][outputX].ansi24BitTruecolorFG = asciiGraphics.colorBuffer[y][x].ansi24BitTruecolorFG;
		this->colorBuffer[outputY][outputX].ansi24BitTruecolorBG = asciiGraphics.colorBuffer[y][x].ansi24BitTruecolorBG;
	}
}

void ConArtist::CADisplayOutputCMD::set_cursor_visibility(bool value) {
	
	if (value) {
		// show the cursor
		CONSOLE_CURSOR_INFO curInfo;
		GetConsoleCursorInfo(this->consoleOutputHandle, &curInfo);
		curInfo.bVisible = TRUE;
		SetConsoleCursorInfo(this->consoleOutputHandle, &curInfo);
		wprintf(CSI L"?25h"); // use the virutal terminal sequence as well
	}
	else {
		// hide the cursor
		CONSOLE_CURSOR_INFO curInfo;
		GetConsoleCursorInfo(this->consoleOutputHandle, &curInfo);
		curInfo.bVisible = FALSE;
		SetConsoleCursorInfo(this->consoleOutputHandle, &curInfo);
		wprintf(CSI L"?25l"); // use the virutal terminal sequence as well
	}
}

ConArtist::CADisplayOutputCMD::CADisplayOutputCMD() {
	//takeOverCMD();
}

ConArtist::CADisplayOutputCMD::~CADisplayOutputCMD() {
	releaseCMD();
}

void ConArtist::CADisplayOutputCMD::takeOverCMD() {
	// allow for wprintf to output UTF-16 encoded unicode to stdout
	_setmode(_fileno(stdout), _O_U16TEXT);

	this->consoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// Set output mode to handle virtual terminal sequences
	DWORD dwMode = 0;
	GetConsoleMode(this->consoleOutputHandle, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(this->consoleOutputHandle, dwMode);

	// resize/maximize our buffers
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
	GetConsoleScreenBufferInfo(this->consoleOutputHandle, &consoleScreenBufferInfo);
	resizeBuffer(&consoleScreenBufferInfo.dwMaximumWindowSize);

	wprintf(CSI L"?1049h");	// switch to alternate buffer
	set_cursor_visibility(false); // hide the cursor
}

void ConArtist::CADisplayOutputCMD::releaseCMD() {
	// reset the output mode of stdout
	_setmode(_fileno(stdout), _O_TEXT);

	wprintf(CSI L"?1049l");	// switch to main buffer
	set_cursor_visibility(true); // show the cursor
}

void ConArtist::CADisplayOutputCMD::pushOutput(CADisplayBuffer& asciiGraphics, ConArtist::ANSIColorDepth maxAllowedColorDepth) {

	// check what size our display is
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
	GetConsoleScreenBufferInfo(this->consoleOutputHandle, &consoleScreenBufferInfo);

	// if our display size has changed since the last output, resize our buffers to fit the new size
	if (consoleScreenBufferInfo.dwMaximumWindowSize.X != this->currentWindowSize.X || 
		consoleScreenBufferInfo.dwMaximumWindowSize.Y != this->currentWindowSize.Y) 
	{
		this->resizeBuffer(&consoleScreenBufferInfo.dwMaximumWindowSize);
	}


	// find the top-left coordinates we need to draw our ASCIIGraphics at to be centered
	short xOrigin = std::max(0, (consoleScreenBufferInfo.dwMaximumWindowSize.X - asciiGraphics.width) / 2);
	short yOrigin = std::max(0, (consoleScreenBufferInfo.dwMaximumWindowSize.Y - asciiGraphics.height) / 2);



	// draw the screen using a mix of Win32 API and Virtual Terminal Sequences (VTS)
	DWORD dwBytesWritten;
	for (short y = 0; y < asciiGraphics.height; y++) {
		for (short x = 0; x < asciiGraphics.width; x++) {

			// the color depth we'll be drawing at determines whether we'll use Win32 API or VTS
			ConArtist::ANSIColorDepth currentColorDepth = std::min(asciiGraphics.colorBuffer[y][x].preferredColorDepth, maxAllowedColorDepth);

			// if there are no changes in this cell, skip it
			if (!this->cellNeedsUpdate(x, y, asciiGraphics, currentColorDepth, xOrigin + x, yOrigin + y)) {
				continue;
			}

			// draw 4-bit color using Win32 API, it's more performant than using VTS
			if (currentColorDepth == ConArtist::ANSI_4BIT_COLOR_DEPTH) {

				// batch consecutive cells of the same color (more performant than drawing each cell)
				short x2 = x;
				std::u16string outputString;
				std::vector<WORD> outputColors;
				while (x2 < asciiGraphics.width &&
				       this->cellNeedsUpdate(x2, y, asciiGraphics, std::min(asciiGraphics.colorBuffer[y][x2].preferredColorDepth, maxAllowedColorDepth), xOrigin + x2, yOrigin + y) &&
				       std::min(asciiGraphics.colorBuffer[y][x2].preferredColorDepth, maxAllowedColorDepth) == currentColorDepth)
				{
					outputString.append(utf8::utf8to16(utf8::utf32to8(std::u32string(1, asciiGraphics.textBuffer[y][x2]))));
					WORD foreground = this->ansi4BitSGRtoWin32Attribute[asciiGraphics.colorBuffer[y][x2].ansi4BitColorFG];
					WORD background = this->ansi4BitSGRtoWin32Attribute[asciiGraphics.colorBuffer[y][x2].ansi4BitColorBG];
					outputColors.push_back(foreground | background);
					storeCell(x2, y, asciiGraphics, currentColorDepth, xOrigin + x2, yOrigin + y);
					x2++;
				}
				x2--;

				// print output to the console
				WriteConsoleOutputCharacterW(this->consoleOutputHandle,
				                             (wchar_t*)outputString.c_str(),
				                             outputString.size(),
				                             { (short)(xOrigin + x), (short)(yOrigin + y) },
				                             &dwBytesWritten);

				WriteConsoleOutputAttribute(this->consoleOutputHandle,
				                            outputColors.data(),
				                            outputColors.size(),
				                            { (short)(xOrigin + x), (short)(yOrigin + y) },
				                            & dwBytesWritten);

				x = x2;	// update `x` to continue just after the cells we just printed
			}

			// draw 8-bit color using VTS
			else if (currentColorDepth == ConArtist::ANSI_8BIT_COLOR_DEPTH) {

				// batch consecutive cells of the same color (more performant than drawing each cell)
				short x2 = x;
				std::u16string outputString;
				while (x2 < asciiGraphics.width &&
				      this->cellNeedsUpdate(x2, y, asciiGraphics, std::min(asciiGraphics.colorBuffer[y][x2].preferredColorDepth, maxAllowedColorDepth), xOrigin + x2, yOrigin + y) &&
				      std::min(asciiGraphics.colorBuffer[y][x2].preferredColorDepth, maxAllowedColorDepth) == currentColorDepth)
				{
					outputString.append(utf8::utf8to16(utf8::utf32to8(std::u32string(1, asciiGraphics.textBuffer[y][x2]))));
					storeCell(x2, y, asciiGraphics, currentColorDepth, xOrigin + x2, yOrigin + y);
					x2++;
				}
				x2--;

				// print output to the console
				wprintf(CSI L"%d;%dH", yOrigin + y + 1, xOrigin + x + 1);	// position the cursor
				wprintf(CSI L"38;5;%dm",	// set foreground 8-bit color
					asciiGraphics.colorBuffer[y][x].ansi8BitColorFG);
				wprintf(CSI L"48;5;%dm", // set background 8-bit color
					asciiGraphics.colorBuffer[y][x].ansi8BitColorBG);
				wprintf((wchar_t*)outputString.c_str()); // print output

				x = x2;	// update `x` to continue just after the cells we just printed
			}
			

			// draw 24-bit/Truecolor color using VTS 
			else if (currentColorDepth == ConArtist::ANSI_24BIT_COLOR_DEPTH) {

				// batch consecutive cells of the same color (more performant than drawing each cell)
				short x2 = x;
				std::u16string outputString;
				while (x2 < asciiGraphics.width &&
				       this->cellNeedsUpdate(x2, y, asciiGraphics, std::min(asciiGraphics.colorBuffer[y][x2].preferredColorDepth, maxAllowedColorDepth), xOrigin + x2, yOrigin + y) &&
				       std::min(asciiGraphics.colorBuffer[y][x2].preferredColorDepth, maxAllowedColorDepth) == currentColorDepth)
				{
					outputString.append(utf8::utf8to16(utf8::utf32to8(std::u32string(1, asciiGraphics.textBuffer[y][x2]))));
					storeCell(x2, y, asciiGraphics, currentColorDepth, xOrigin + x2, yOrigin + y);
					x2++;
				}
				x2--;

				// print output to the console
				wprintf(CSI L"%d;%dH", yOrigin + y + 1, xOrigin + x + 1);	// position the cursor
				wprintf(CSI L"38;2;%d;%d;%dm",	// set foreground RGB color
				        asciiGraphics.colorBuffer[y][x].ansi24BitTruecolorFG.r,
				        asciiGraphics.colorBuffer[y][x].ansi24BitTruecolorFG.g,
				        asciiGraphics.colorBuffer[y][x].ansi24BitTruecolorFG.b);
				wprintf(CSI L"48;2;%d;%d;%dm", // set background RGB color
				        asciiGraphics.colorBuffer[y][x].ansi24BitTruecolorBG.r,
				        asciiGraphics.colorBuffer[y][x].ansi24BitTruecolorBG.g,
				        asciiGraphics.colorBuffer[y][x].ansi24BitTruecolorBG.b);
				wprintf((wchar_t*)outputString.c_str()); // print output

				x = x2;	// update `x` to continue just after the cells we just printed
			}

		}
	}

	this->fullRedraw = false;

}
