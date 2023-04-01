#pragma once

#include <string>
#include <Windows.h>

namespace Soil {
	class ASCIISprite {
	private:

	public:
		std::string text;
		WORD color;
		ASCIISprite(std::string text, WORD color);
	};
}