#define SDL_MAIN_HANDLED

#include "Application.h"
#include "Tetris.h"
#include "Level.h"
#include "Assets.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <stdbool.h>

Application application;

int nextUpdate;

bool Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0 || TTF_Init() != 0)
	{
	    return false;
	}
	else
	{
		SDL_DisplayMode dm;
		SDL_GetCurrentDisplayMode(0, &dm);
		int screenWidth = dm.w;
		int screenHeight = dm.h;

		application.viewport.w = LEVELWIDTH * CELLSIZE + 256;
		application.viewport.h = LEVELHEIGHT * CELLSIZE;
		application.window = SDL_CreateWindow("cock", (screenWidth - application.viewport.w) / 2, (screenHeight - application.viewport.h) / 2, application.viewport.w, application.viewport.h, 0);
		application.renderer = SDL_CreateRenderer(application.window, -1, SDL_RENDERER_ACCELERATED);
		
		return true;
	}
}

void Exit(void)
{
    SDL_DestroyRenderer(application.renderer);
    SDL_DestroyWindow(application.window);
    SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (Initialize())
	{
		// Run game
		InitializeAssets();
		InitializeTetris();
		
		while (!ShouldExitTetris())	
		{
			if (SDL_GetWindowFlags(application.window) | SDL_WINDOW_INPUT_FOCUS)
			{
				UpdateTetris();	
				SDL_RenderSetViewport(application.renderer, &application.viewport);
				DrawTetris();
			}
	
			SDL_Delay(16);
		}

		UnloadAssets();
		Exit();
		return 0;
	}
	
	return -1;
}
