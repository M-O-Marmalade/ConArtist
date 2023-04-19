#include "ASCIIOutputCMD.h"

#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include "utfcpp/source/utf8.h"

#define ESC L"\x1b"
#define CSI L"\x1b["

Soil::ASCIIOutputCMD::ASCIIOutputCMD() {

	// allow for wprintf to output UTF-16 encoded unicode
	_setmode(_fileno(stdout), _O_U16TEXT);

	this->consoleOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// Set output mode to handle virtual terminal sequences
	DWORD dwMode = 0;
	GetConsoleMode(this->consoleOutputHandle, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(this->consoleOutputHandle, dwMode);

	wprintf(CSI L"?1049h");	// switch to alternate buffer
	wprintf(CSI L"?25l");	// hide the cursor
}

Soil::ASCIIOutputCMD::~ASCIIOutputCMD() {
	wprintf(CSI L"?1049l");	// switch to main buffer
	wprintf(CSI L"?25h");	// show the cursor
}

void Soil::ASCIIOutputCMD::pushOutput(ASCIIGraphics& asciiGraphics, Soil::ANSIColorDepth maxAllowedColorDepth) {

	// check what size our window is
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
	GetConsoleScreenBufferInfo(this->consoleOutputHandle, &consoleScreenBufferInfo);

	if (consoleScreenBufferInfo.dwMaximumWindowSize.X != this->currentWindowSize.X || 
		consoleScreenBufferInfo.dwMaximumWindowSize.Y != this->currentWindowSize.Y) 
	{

		// resize our buffer to fit the window size
		SetConsoleScreenBufferSize(this->consoleOutputHandle, consoleScreenBufferInfo.dwMaximumWindowSize);

		// clear the buffer of any leftover characters from the previous size
		DWORD dwBytesWritten;
		FillConsoleOutputCharacter(this->consoleOutputHandle,
		                           ' ',
		                           consoleScreenBufferInfo.dwMaximumWindowSize.X * consoleScreenBufferInfo.dwMaximumWindowSize.Y, 
		                           { 0,0 }, 
		                           & dwBytesWritten);

		// redraw the full screen
		asciiGraphics.resetTextObservers(true);
		asciiGraphics.resetColorObservers(true);

		// store the size for comparison the next time we check this `if` statement
		this->currentWindowSize = consoleScreenBufferInfo.dwMaximumWindowSize;
	}


	// find the top-left coordinates we need to draw our ASCIIGraphics at to be centered
	short xOrigin = std::max(0, (consoleScreenBufferInfo.dwMaximumWindowSize.X - asciiGraphics.width) / 2);
	short yOrigin = std::max(0, (consoleScreenBufferInfo.dwMaximumWindowSize.Y - asciiGraphics.height) / 2);



	// draw the screen using a mix of Win32 API and Virtual Terminal Sequences (VTS)
	DWORD dwBytesWritten;
	for (short y = 0; y < asciiGraphics.height; y++) {

		// if there are no changes in this row, skip it
		if (!asciiGraphics.changedTextRows[y] && !asciiGraphics.changedColorRows[y]) {
			continue;
		}

		for (short x = 0; x < asciiGraphics.width; x++) {

			// if there are no changes in this cell, skip it
			if (!asciiGraphics.changedTextCells[y][x] && !asciiGraphics.changedColorCells[y][x]) {
				continue;
			}

			// the color depth we'll be drawing at determines whether we'll use Win32 API or VTS
			Soil::ANSIColorDepth currentColorDepth = std::min(asciiGraphics.colorBuffer[y][x].preferredColorDepth, maxAllowedColorDepth);

			// draw 4-bit color using Win32 API, it's more performant than using VTS
			if (currentColorDepth == Soil::ANSI_4BIT_COLOR) {
				
				// batch consecutive cells of the same color (more performant than drawing each cell)
				//short x2 = x;
				//std::vector<CHAR_INFO> outputVector;
				//while (x2 < asciiGraphics.width 
				//       && (asciiGraphics.changedTextCells[y][x] || asciiGraphics.changedColorCells[y][x])
				//       && std::min(asciiGraphics.colorBuffer[y][x2].preferredColorDepth, maxAllowedColorDepth) == currentColorDepth
				//       && asciiGraphics.colorBuffer[y][x2].ansi4BitColor == asciiGraphics.colorBuffer[y][x].ansi4BitColor)
				//{
				//	CHAR_INFO charInfoToWrite;
				//	charInfoToWrite.Attributes = asciiGraphics.colorBuffer[y][x2].ansi4BitColor;
				//	charInfoToWrite.Char.UnicodeChar = (wchar_t)utf8::utf8to16(utf8::utf32to8(std::u32string(1, asciiGraphics.textBuffer[y][x2])))[0];
				//	outputVector.push_back(charInfoToWrite);
				//	x2++;
				//}
				//x2--;

				//// print output to the console
				//SMALL_RECT drawCoordinates;
				//drawCoordinates.Left = xOrigin + x;
				//drawCoordinates.Right = xOrigin + x2;
				//drawCoordinates.Top = yOrigin + y;
				//drawCoordinates.Bottom = yOrigin + y;
				//WriteConsoleOutputW(this->consoleOutputHandle,
				//		            &outputVector[0],
				//		            { x2, 1},
				//		            { 0, 0 },
				//		            &drawCoordinates);


				// batch consecutive cells of the same color (more performant than drawing each cell)
				short x2 = x;
				std::u16string outputString;
				std::vector<WORD> outputColors;
				while (x2 < asciiGraphics.width
					&& (asciiGraphics.changedTextCells[y][x] || asciiGraphics.changedColorCells[y][x])
					&& std::min(asciiGraphics.colorBuffer[y][x2].preferredColorDepth, maxAllowedColorDepth) == currentColorDepth
					&& asciiGraphics.colorBuffer[y][x2].ansi4BitColor == asciiGraphics.colorBuffer[y][x].ansi4BitColor)
				{
					outputString.append(utf8::utf8to16(utf8::utf32to8(std::u32string(1, asciiGraphics.textBuffer[y][x2]))));
					outputColors.push_back(asciiGraphics.colorBuffer[y][x2].ansi4BitColor);
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
			else if (currentColorDepth == Soil::ANSI_8BIT_COLOR) {

				// batch consecutive cells of the same color (more performant than drawing each cell)
				short x2 = x;
				std::u16string outputString;
				while (x2 < asciiGraphics.width
					    && std::min(asciiGraphics.colorBuffer[y][x2].preferredColorDepth, maxAllowedColorDepth) == currentColorDepth
				        && asciiGraphics.colorBuffer[y][x2].ansi8BitColorFG == asciiGraphics.colorBuffer[y][x].ansi8BitColorFG
					    && asciiGraphics.colorBuffer[y][x2].ansi8BitColorBG == asciiGraphics.colorBuffer[y][x].ansi8BitColorBG)
				{
					outputString.append(utf8::utf8to16(utf8::utf32to8(std::u32string(1, asciiGraphics.textBuffer[y][x2]))));
					x2++;
				}
				x2--;

				// print output to the console
				wprintf(CSI L"%d;%dH", yOrigin + y + 1, xOrigin + x + 1);	// position the cursor
				wprintf(CSI L"38;5;%dm",	// set foreground 8-bit color
					asciiGraphics.colorBuffer[y][x2].ansi8BitColorFG);
				wprintf(CSI L"48;5;%dm", // set background 8-bit color
					asciiGraphics.colorBuffer[y][x2].ansi8BitColorBG);
				wprintf((wchar_t*)outputString.c_str()); // print output

				x = x2;	// update `x` to continue just after the cells we just printed
			}
			

			// draw 24-bit/Truecolor color using VTS 
			else if (currentColorDepth == Soil::ANSI_24BIT_COLOR) {

				// batch consecutive cells of the same color (more performant than drawing each cell)
				short x2 = x;
				std::u16string outputString;
				while (x2 < asciiGraphics.width
				       && std::min(asciiGraphics.colorBuffer[y][x2].preferredColorDepth, maxAllowedColorDepth) == currentColorDepth
				       && asciiGraphics.colorBuffer[y][x2].ansi24BitTruecolorFG == asciiGraphics.colorBuffer[y][x].ansi24BitTruecolorFG
				       && asciiGraphics.colorBuffer[y][x2].ansi24BitTruecolorBG == asciiGraphics.colorBuffer[y][x].ansi24BitTruecolorBG)
				{
					outputString.append(utf8::utf8to16(utf8::utf32to8(std::u32string(1, asciiGraphics.textBuffer[y][x2]))));
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




	asciiGraphics.resetTextObservers(false);
	asciiGraphics.resetColorObservers(false);
}
