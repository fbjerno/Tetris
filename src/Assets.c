#include "Assets.h"
#include "Application.h"

#define LoadTexture(var, path) {SDL_Surface* temp_surface = IMG_Load(path); *var = SDL_CreateTextureFromSurface(application.renderer, temp_surface); SDL_FreeSurface(temp_surface);}

Assets* assets;
Assets _assets;

SDL_Texture* tetrominoBase = NULL;
SDL_Texture* tetrominoHighlight = NULL;

void InitializeAssets(void)
{
	assets = &_assets;
	LoadTexture(&(_assets.tetrominoBase), "assets/TetrominoBase.png");
	LoadTexture(&(_assets.tetrominoHighlight), "assets/TetrominoHighlight.png");
}

void UnloadAssets(void)
{
	SDL_DestroyTexture(tetrominoBase);
	SDL_DestroyTexture(tetrominoHighlight);
}