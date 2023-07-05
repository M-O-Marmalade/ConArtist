// This class is currently unneeded/unused.

#pragma once
#include "ASCIIGraphics.h"

namespace Soil {
	class ASCIIOutput
	{
		virtual void pushOutput(ASCIIGraphics& asciiGraphics, Soil::ANSIColorDepth maxAllowedColorDepth) = 0;
	};
}