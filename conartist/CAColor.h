// The CAColor class is used to define and store multi-color-depth ANSI terminal colors.
// It stores 4-bit, 8-bit, and 24-bit color data all at once, and defines which color depth it wants to be rendered at.
// The color will render at its preferredColorDepth, unless the maxAllowedColorDepth of the CADisplayOutput class that's rendering it is lower.
// If you define, for example, a 24-bit color, it's a good idea to also set the 8-bit and 4-bit colors to a similar color, so that the color will look similar/correct even if it ends up being output at those lower color depths.
// For a reference of all 4-bit and 8-bit ANSI color palettes, see https://en.wikipedia.org/wiki/ANSI_escape_code#Colors.


#pragma once
#include <cstdint>

#include "ANSITrueColor.h"


namespace ConArtist {

	constexpr auto ANSI_4BIT_DEFAULT = 0;

	// base colors
	constexpr auto ANSI_4BIT_BLACK =   30;
	constexpr auto ANSI_4BIT_RED =     31;
	constexpr auto ANSI_4BIT_GREEN =   32;
	constexpr auto ANSI_4BIT_YELLOW =  33;
	constexpr auto ANSI_4BIT_BLUE =    34;
	constexpr auto ANSI_4BIT_MAGENTA = 35;
	constexpr auto ANSI_4BIT_CYAN =    36;
	constexpr auto ANSI_4BIT_WHITE =   37;

	// bright colors
	constexpr auto ANSI_4BIT_GRAY =           90;
	constexpr auto ANSI_4BIT_BRIGHT_RED =     91;
	constexpr auto ANSI_4BIT_BRIGHT_GREEN =   92;
	constexpr auto ANSI_4BIT_BRIGHT_YELLOW =  93;
	constexpr auto ANSI_4BIT_BRIGHT_BLUE =    94;
	constexpr auto ANSI_4BIT_BRIGHT_MAGENTA = 95;
	constexpr auto ANSI_4BIT_BRIGHT_CYAN =    96;
	constexpr auto ANSI_4BIT_BRIGHT_WHITE =   97;

	
	enum ANSIColorDepth {
		ANSI_4BIT_COLOR_DEPTH = 0,
		ANSI_8BIT_COLOR_DEPTH = 1,
		ANSI_24BIT_COLOR_DEPTH = 2
	};

	class CAColor {
	public:
		uint_fast8_t ansi4BitColorFG;
		uint_fast8_t ansi4BitColorBG;
		uint_fast8_t ansi8BitColorFG;
		uint_fast8_t ansi8BitColorBG;
		ConArtist::ANSITrueColor ansi24BitTruecolorFG;
		ConArtist::ANSITrueColor ansi24BitTruecolorBG;

		ConArtist::ANSIColorDepth preferredColorDepth;

		CAColor(uint_fast8_t ansi4BitColorFG = ANSI_4BIT_WHITE,  // default/no styling
		           uint_fast8_t ansi4BitColorBG = ANSI_4BIT_DEFAULT,  // default/no styling
		           uint_fast8_t ansi8BitColorFG = 15, // white
		           uint_fast8_t ansi8BitColorBG = 0,  // black
		           ConArtist::ANSITrueColor ansi24BitTruecolorFG = ConArtist::ANSITrueColor(255, 255, 255),
		           ConArtist::ANSITrueColor ansi24BitTruecolorBG = ConArtist::ANSITrueColor(0, 0, 0),
		           ConArtist::ANSIColorDepth preferredColorDepth = ANSI_4BIT_COLOR_DEPTH);
		bool operator==(CAColor& other);
		bool operator!=(CAColor& other);



	};
}
