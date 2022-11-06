#ifndef TETRIS_INPUT
#define TETRIS_INPUT

#include "SDL.h"
#include <stdbool.h>

typedef struct TetrisInput
{
	bool left;
	bool right;
	bool down;
	bool place;
} TetrisInput;

TetrisInput InputFromKeyboardState(const Uint8* keyboardState);

#endif
