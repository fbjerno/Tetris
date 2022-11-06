#include "Assets.h"
#include "Application.h"
#include "SDL.h"
#define LoadTexture(var, path) {SDL_Surface* temp_surface = IMG_Load(path); var = SDL_CreateTextureFromSurface(application.renderer, temp_surface); SDL_FreeSurface(temp_surface);}
#define LoadFont(var, path, pt) var = FC_CreateFont(); FC_LoadFont(var, application.renderer, path, pt, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);

Assets assets;

void InitializeAssets(void)
{
  	LoadTexture(assets.tetrominoBase, "assets/TetrominoBase.png");
  	LoadTexture(assets.tetrominoHighlight, "assets/TetrominoHighlight.png");
	LoadFont(assets.titleFont, "assets/quinquefive.ttf", 16);
	LoadFont(assets.textFont, "assets/quinquefive.ttf", 12);
}

void UnloadAssets(void)
{
	SDL_DestroyTexture(assets.tetrominoBase);
  	SDL_DestroyTexture(assets.tetrominoHighlight);
	FC_FreeFont(assets.titleFont);
	FC_FreeFont(assets.textFont);
}
