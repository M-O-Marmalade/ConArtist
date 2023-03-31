#pragma once
#include "ASCIIGraphics.h"

class ASCIIOutput
{
	virtual void pushOutput(ASCIIGraphics& asciiGraphics) = 0;
};

