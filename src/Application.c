#define SDL_MAIN_HANDLED

#include "Application.h"
#include "Tetris.h"
#include "Level.h"
#include "Assets.h"
#include <SDL.h>
#include <stdbool.h>

Application application;

int nextUpdate;

bool Initialize(int width, int height)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		return false;
	}
	else
	{
		SDL_DisplayMode dm;
		SDL_GetCurrentDisplayMode(0, &dm);
		int screenWidth = dm.w;
		int screenHeight = dm.h;
		
		SDL_Window* window = SDL_CreateWindow("cock", (screenWidth - width) / 2, (screenHeight - height) / 2, width, height, 0);
		SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		
		application = (Application){window, renderer};
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
	if (Initialize(LEVELWIDTH * CELLSIZE, LEVELHEIGHT * CELLSIZE))
	{
		// Run game
		
		InitializeAssets();
		InitializeTetris();
		
		while (!ShouldExitTetris())	
		{
			Uint64 currentTime = SDL_GetTicks64();
			if (currentTime > nextUpdate)
			{
				nextUpdate = currentTime + 16;
				
				UpdateTetris();
				DrawTetris();
				// TODO: Experiment with using WaitEvent instead of this solution
				SDL_Delay(16);
			}
		}
		
		Exit();
		return 0;
	}
	
	return -1;
}