// This class is currently unneeded/unused.

#pragma once
#include "CADisplayBuffer.h"

namespace ConArtist {
	class CADisplayOutput
	{
		virtual void pushOutput(CADisplayBuffer& asciiGraphics, ConArtist::ANSIColorDepth maxAllowedColorDepth) = 0;
	};
}