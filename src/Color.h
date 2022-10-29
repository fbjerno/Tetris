#ifndef TETRIS_COLOR
#define TETRIS_COLOR

#define Color(r, g, b, a) (Color){r, g, b, a}
#define ColorComponents(color) color.r, color.g, color.b, color.a
#define ColorComponentsRGB(color) color.r, color.g, color.b

#include "SDL_stdinc.h"

typedef struct Color
{
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
} Color;

#endif