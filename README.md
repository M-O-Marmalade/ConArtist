# Soil
A cross-platform C++ ASCII/Unicode terminal game development library.

## Ethos
Eases cross-platform terminal game development, without taking low-level control away from, or enforcing any opinionated methodologies or workflows onto, the developer.

## Features
- Classes to aid in creating and drawing ASCII/Unicode text graphics.
- Full ANSI color support: 4-bit, 8-bit, and 24-bit/Truecolor.
- Smart rendering routines (lazy updating, color-batching) allows for framerates as high as 1000's per-second, depending on the graphics being drawn.
- Interchangeable rendering backends/outputs (currently only Win32 Console is implemented).
- Multithreaded user input system for detecting mouse/keyboard input reliably.
