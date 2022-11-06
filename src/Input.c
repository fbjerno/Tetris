#include "Input.h"

TetrisInput InputFromKeyboardState(const Uint8* keyboardState)
{
	TetrisInput input;
	input.left = keyboardState[SDL_SCANCODE_LEFT];
	input.right = keyboardState[SDL_SCANCODE_RIGHT];
	input.down = keyboardState[SDL_SCANCODE_DOWN];
	input.place = keyboardState[SDL_SCANCODE_SPACE] || keyboardState[SDL_SCANCODE_C];
	
	return input;
}
