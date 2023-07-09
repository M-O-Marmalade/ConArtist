<p align="center">
    <img src="https://raw.githubusercontent.com/M-O-Marmalade/ConArtist/main/logo.png" width="216">
</p>

<h1 align="center">ConArtist</h1>

C++ Console/Terminal game development library. Decouples game code from OS code, and provides performant and easy-to-use systems.

## Features
- Unicode support (recommended to use only monospaced characters)
- Full ANSI color support, with fallback to lower color depths.: 4-bit, 8-bit, and 24-bit "Truecolor"
- Framerates as high as 1000's per-second, thanks to smart rendering routines (lazy updating, color-batching)
- Keyboard/mouse button input detection system
- Windows support (support for Linux and MacOS are planned)

## Setup
1. Open Visual Studio.
2. Create a new project.
3. Filter the templates by "C++" language, and "Windows" platform.
4. Choose the "Empty Project" template for Windows.
5. Name your project, choose a save location for the project, and then create the project.
6. Download ConArtist's [latest release](https://github.com/M-O-Marmalade/ConArtist/releases/download/1.0/conartist.zip) and extract the contents of the .zip file into your project's folder.
7. Add the ConArtist source files to your project via ***Project > Add Exisiting Item...***
	- Note: The files in the `utfcpp` folder of the ConArtist library do not need to be added. Technically, only the .cpp files of the ConArtist library need to be added to the project.
8. Create your program's main C++ file via ***Project > Add New Item...***
9. Choose "C++ File (.cpp)", and name the file whatever you like (recommendation: "main.cpp").
	- At this point, your Project folder should look something like this (plus a few extra files Visual Studio creates automatically):
```
Project Folder
|- conartist (folder containing .h and .cpp files, plus the "utfcpp" folder)
|- main.cpp
|- Project.sln
```   
10. Change this Project setting:
	- ***Project > Properties > C/C++ > Command Line > Additional Options*** = `/Zc:__cplusplus`
		- This is required due to this library's use of the C++ 11 functions from the UTF8-CPP library.
		- See [this article](https://learn.microsoft.com/en-us/cpp/build/reference/zc-cplusplus?view=msvc-170) for more details about this setting.
11. Include the ConArtist library, and create the main function, where you'll write your program.
```cpp
#include "conartist/ConArtist.h"

int main() {

}
```
12. You're ready to create games with ConArtist!

## Examples
Here is a simple "Hello World" program for Windows.

```cpp
#include "conartist/ConArtist.h"

using namespace std::chrono_literals;

int main()
{
	// Create a display buffer 60 cells wide, and 15 cells high, where we will draw our graphics.
	// Also create a display output, which will push our graphics to the terminal.
	ConArtist::CADisplayBuffer caDisplayBuffer(60, 15);
	ConArtist::CADisplayOutputCMD caDisplayOutputCMD;

	// Set the text and background colors to a 4-bit white and 4-bit blue, respectively
	caDisplayBuffer.fillColor(0, 0, 80, 25, ConArtist::CAColor(ConArtist::ANSI_4BIT_WHITE, ConArtist::ANSI_4BIT_BLUE));

	// Place a string into the center of the screen buffer
	std::u32string hello = U"Hello ConArtist!";
	int xCoord = caDisplayBuffer.width / 2 - hello.size() / 2;
	int yCoord = caDisplayBuffer.height / 2;
	caDisplayBuffer.drawText(xCoord, yCoord, hello);

	// Display the screen buffer to the terminal
	caDisplayOutputCMD.pushOutput(caDisplayBuffer);

	// Show the terminal for 5 seconds before the program finishes and closes automatically
	std::this_thread::sleep_for(5s);
}
```


- The [SNEK](https://github.com/M-O-Marmalade/SNEK) project is built using ConArtist.

## Notes
- Doesn't currently play well with non-monospaced characters (e.g. full-width [Katakana](https://en.wikipedia.org/wiki/Katakana)). Monospaced alternatives should be used instead (e.g. [half-width kana](https://en.wikipedia.org/wiki/Half-width_kana))
- When working with Unicode ASCII-style graphics, the use of UTF-32 encoded `u32string` is recommended over UTF-8 encoded `string`, as it simplifies character indexing operations. ConArtist uses `u32string` internally, and converts UTF-8 encoded `string` and UTF-16 encoded `u16string` to UTF-32 encoded `u32string` when these datatypes are passed as parameters to ConArtist class functions. Despite this, ConArtist's memory & performance footprints are very minimal. The choice of UTF-32 encoding simplifies Unicode ASCII-style game development and reduces potential bugs significantly.
- The `__cplusplus` preprocessor macro must report `201103L` (C++ 11) or later, due to the use of the C++ 11 functions from the UTF8-CPP library.
	- See step 10 in the Setup instructions above for instructions on how to accomplish this in Visual Studio.

## Dependencies / Credits
- The [UTF8-CPP](https://github.com/nemtrif/utfcpp) library is used to convert between string encodings.
