#pragma once

#include <cstdint>
#include <string>
#include <Windows.h>

#include "ASCIIColor.h"

namespace Soil {
	class ASCIISprite {
	private:

	public:
		std::u32string text;
		Soil::ASCIIColor color;
		ASCIISprite(std::string text, Soil::ASCIIColor color);
		ASCIISprite(std::u32string text, Soil::ASCIIColor color);
	};
}