// Stores Unicode text and ANSI color data for a ASCII-style text sprite.
// Only stores a single color, so the entire sprite is one color.
// A variant that stores multiple colors could be implemented rather easily.

#pragma once

#include <cstdint>
#include <string>
#include <Windows.h>

#include "CAColor.h"

namespace ConArtist {
	class CAMonoSprite {
	private:

	public:
		std::u32string text;
		ConArtist::CAColor color;
		CAMonoSprite(std::string text, ConArtist::CAColor color);
		CAMonoSprite(std::u32string text, ConArtist::CAColor color);
	};
}