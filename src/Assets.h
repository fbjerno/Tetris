#ifndef TETRIS_ASSETS
#define TETRIS_ASSETS
#include "SDL_surface.h"
#include "SDL_image.h"

typedef struct Assets
{
	SDL_Texture* tetrominoBase;
	SDL_Texture* tetrominoHighlight;
} Assets;

extern Assets* assets;

void InitializeAssets(void);
void UnloadAssets(void);

#endif