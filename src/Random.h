#ifndef TETRIS_RANDOM
#define TETRIS_RANDOM
#include <stdlib.h>

int RandomInt(int min, int max)
{
	// Stolen from https://www.geeksforgeeks.org/generating-random-number-range-c/
	return (rand() % (max - min + 1)) + min;
}

#endif