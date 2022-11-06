#ifndef TETRIS_RANDOM
#define TETRIS_RANDOM
#include <stdlib.h>
#include <time.h>

int RandomInt(int min, int max)
{
    static bool set = false;
    if (!set)
    {
        srand((unsigned int)time(NULL));
	set = true;
    }
    // Stolen from https://www.geeksforgeeks.org/generating-random-number-range-c/
    return (rand() % (max - min + 1)) + min;
}

#endif
