#pragma once

#include <string>
#include <Windows.h>

class ASCIISprite {
private:

public:
	std::string text;
	WORD color;
	ASCIISprite(std::string text, WORD color);
};