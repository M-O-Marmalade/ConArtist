// Defines a 24-bit ANSI color, or ANSI "TrueColor". Used in the ASCIIColor class.

#pragma once
#include <cstdint>

namespace ConArtist {
	class ANSITrueColor {
	public:
		uint_fast8_t r = 0;
		uint_fast8_t g = 0;
		uint_fast8_t b = 0;

		ANSITrueColor(uint_fast8_t r = 0, uint_fast8_t g = 0, uint_fast8_t b = 0) : r{ r }, g{ g }, b{ b } {};
		bool operator==(ANSITrueColor& other) {
			if (this->r == other.r && this->g == other.g && this->b == other.b) {
				return true;
			}
			return false;
		}
		bool operator!=(ANSITrueColor& other) {
			if (this->r == other.r && this->g == other.g && this->b == other.b) {
				return false;
			}
			return true;
		}
	};
}
