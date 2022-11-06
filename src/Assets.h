#ifndef TETRIS_ASSETS
#define TETRIS_ASSETS
#include "SDL_surface.h"
#include "SDL_image.h"
#include "SDL_FontCache.h"

typedef struct Assets
{
	SDL_Texture* tetrominoBase;
	SDL_Texture* tetrominoHighlight;
	FC_Font* titleFont;
	FC_Font* textFont;
} Assets;

extern Assets assets;

void InitializeAssets(void);
void UnloadAssets(void);

#endif
