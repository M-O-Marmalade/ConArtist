#pragma once
#include "ASCIIOutput.h"

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <vector>

namespace Soil {
    class ASCIIOutputCMD : public ASCIIOutput {
    private:
        HANDLE originalConsoleHandle = 0;
        HANDLE gameConsoleHandle = 0;
    public:
        ASCIIOutputCMD();
        ~ASCIIOutputCMD();
        void pushOutput(ASCIIGraphics& asciiGraphics);
    };
}
