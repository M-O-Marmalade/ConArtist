// the ASCIIOutputCMD class is used to display console/terminal graphics on Microsoft Windows.

#pragma once
#include "ASCIIOutput.h"

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <map>

namespace Soil {
    class ASCIIOutputCMD : public ASCIIOutput {
    private:
        std::map<uint_fast8_t, WORD> ansi4BitSGRtoWin32Attribute = {
            
            // blank/default
            {0, 0},

            // foregrounds
            {30, 0},
            {31, FOREGROUND_RED},
            {32, FOREGROUND_GREEN},
            {33, FOREGROUND_RED | FOREGROUND_GREEN},
            {34, FOREGROUND_BLUE},
            {35, FOREGROUND_BLUE | FOREGROUND_RED},
            {36, FOREGROUND_BLUE | FOREGROUND_GREEN},
            {37, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE},

            // bright foregrounds
            {90, FOREGROUND_INTENSITY | 0},
            {91, FOREGROUND_INTENSITY | FOREGROUND_RED},
            {92, FOREGROUND_INTENSITY | FOREGROUND_GREEN},
            {93, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN},
            {94, FOREGROUND_INTENSITY | FOREGROUND_BLUE},
            {95, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED},
            {96, FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN},
            {97, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE},

            // backgrounds
            {40, 0},
            {41, BACKGROUND_RED},
            {42, BACKGROUND_GREEN},
            {43, BACKGROUND_RED | BACKGROUND_GREEN},
            {44, BACKGROUND_BLUE},
            {45, BACKGROUND_BLUE | BACKGROUND_RED},
            {46, BACKGROUND_BLUE | BACKGROUND_GREEN},
            {47, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE},

            // bright backgrounds
            {100, BACKGROUND_INTENSITY | 0},
            {101, BACKGROUND_INTENSITY | BACKGROUND_RED},
            {102, BACKGROUND_INTENSITY | BACKGROUND_GREEN},
            {103, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN},
            {104, BACKGROUND_INTENSITY | BACKGROUND_BLUE},
            {105, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_RED},
            {106, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN},
            {107, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE}
        };

        // previous frame buffers used for comparison in lazy rendering
        std::vector<std::u32string> textBuffer;
        std::vector<std::vector<Soil::ASCIIColor>> colorBuffer;
        bool fullRedraw = true;

        HANDLE consoleOutputHandle = 0;
        COORD currentWindowSize = {0,0};

        void resizeBuffer(COORD* newDimensions);
        bool cellNeedsUpdate(int x, int y, ASCIIGraphics& asciiGraphics, Soil::ANSIColorDepth currentColorDepth, int outputX, int outputY);
        void storeCell(int x, int y, ASCIIGraphics& asciiGraphics, Soil::ANSIColorDepth currentColorDepth, int outputX, int outputY);

    public:
        ASCIIOutputCMD();
        ~ASCIIOutputCMD();
        void pushOutput(ASCIIGraphics& asciiGraphics, Soil::ANSIColorDepth maxAllowedColorDepth);
    };
}
