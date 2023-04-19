#pragma once
#include "ASCIIOutput.h"

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <vector>

namespace Soil {
    class ASCIIOutputCMD : public ASCIIOutput {
    private:
        HANDLE consoleOutputHandle = 0;
        COORD currentWindowSize = {0,0};
    public:
        ASCIIOutputCMD();
        ~ASCIIOutputCMD();
        void pushOutput(ASCIIGraphics& asciiGraphics, Soil::ANSIColorDepth maxAllowedColorDepth);
    };
}
