#pragma once
#include "ASCIIGraphics.h"

namespace Soil {
	class ASCIIOutput
	{
		virtual void pushOutput(ASCIIGraphics& asciiGraphics) = 0;
	};
}