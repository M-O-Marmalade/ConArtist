#pragma once
#include <cstdint>

#include "ANSITrueColor.h"


namespace Soil {

	constexpr auto ANSI_4BIT_FG_BLUE = 0b00000001;		// text color contains blue.
	constexpr auto ANSI_4BIT_FG_GREEN = 0b00000010;		// text color contains green.
	constexpr auto ANSI_4BIT_FG_RED = 0b00000100;		// text color contains red.
	constexpr auto ANSI_4BIT_FG_BRIGHT = 0b00001000;	// text color is intensified.
	constexpr auto ANSI_4BIT_BG_BLUE = 0b00010000;		// background color contains blue.
	constexpr auto ANSI_4BIT_BG_GREEN = 0b00100000;		// background color contains green.
	constexpr auto ANSI_4BIT_BG_RED = 0b01000000;		// background color contains red.
	constexpr auto ANSI_4BIT_BG_BRIGHT = 0b10000000;	// background color is intensified.
	
	enum ANSIColorDepth {
		ANSI_4BIT_COLOR = 0,
		ANSI_8BIT_COLOR = 1,
		ANSI_24BIT_COLOR = 2
	};

	class ASCIIColor {
	public:
		uint_fast8_t ansi4BitColor;
		uint_fast8_t ansi8BitColorFG;
		uint_fast8_t ansi8BitColorBG;
		Soil::ANSITrueColor ansi24BitTruecolorFG;
		Soil::ANSITrueColor ansi24BitTruecolorBG;

		Soil::ANSIColorDepth preferredColorDepth;

		ASCIIColor(uint_fast8_t ansi4BitColor = 0,
		           uint_fast8_t ansi8BitColorFG = 15, // white
		           uint_fast8_t ansi8BitColorBG = 0, // black
		           Soil::ANSITrueColor ansi24BitTruecolorFG = Soil::ANSITrueColor(255, 255, 255),
		           Soil::ANSITrueColor ansi24BitTruecolorBG = Soil::ANSITrueColor(0, 0, 0),
		           Soil::ANSIColorDepth preferredColorDepth = ANSI_4BIT_COLOR);
		bool operator==(ASCIIColor& other);
		bool operator!=(ASCIIColor& other);

	};
}
