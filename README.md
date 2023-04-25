# Soil
A C++ ASCII/Unicode terminal game development library.

## Ethos
Eases cross-platform terminal game development, without taking low-level control away from, or enforcing any opinionated methodologies or workflows onto, the developer.

## Features
- Classes to aid in creating and drawing ASCII/Unicode text graphics.
- Full ANSI color support: 4-bit, 8-bit, and 24-bit/Truecolor.
- Smart rendering routines (lazy updating, color-batching) allows for framerates as high as 1000's per-second, depending on the graphics being drawn.
- Interchangeable rendering backends/outputs (currently only Win32 Console is implemented).
- User input system on its own thread for reliable and robust mouse/keyboard input detection.

## Notes
- Doesn't currently play well with non-monospaced characters (e.g. full-width [Katakana](https://en.wikipedia.org/wiki/Katakana)). Monospaced alternatives should be used instead (e.g. [half-width kana](https://en.wikipedia.org/wiki/Half-width_kana))
