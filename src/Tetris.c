#include "Tetris.h"
#include "Application.h"
#include "Point.h"
#include "Level.h"
#include "Tetromino.h"
#include "SDL_timer.h"
#include "SDL_events.h"
#include "SDL_keyboard.h"
#include "SDL_keycode.h"
#include <stdbool.h>
#include <string.h>

int speed = 500;
int tetrominosPlaced = 0;

Level level;
Tetromino tetromino;

Uint64 currentTime = 0;
Uint64 lastTime = 0;

bool shouldExit = false;

void InitializeTetris(void)
{
	memset(level.map, 0, sizeof(level.map));
	tetromino = CreateRandomTetromino((Point){LEVELWIDTH / 2, 0});
}

void PlaceTetromino(void)
{
	PlaceTetrominoInLevel(&level, &tetromino);
	tetromino = CreateRandomTetromino((Point){LEVELWIDTH / 2, 0});
	tetrominosPlaced++;
	speed = 500 - tetrominosPlaced * 10;
}

void HandleEventsTetris(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				SDL_Keycode key = event.key.keysym.sym;
				switch (key)
				{
					case SDLK_LEFT:
						TryMoveTetromino(&tetromino, POINT_LEFT, &level);
						break;
					case SDLK_RIGHT:
						TryMoveTetromino(&tetromino, POINT_RIGHT, &level);
						break;
					case SDLK_DOWN:
						TryMoveTetromino(&tetromino, POINT_DOWN, &level);
						break;
					case SDLK_UP:
						TryRotateTetromino(&tetromino, &level);
						break;
					case SDLK_SPACE:
						PlaceTetromino();
						break;
					case SDLK_ESCAPE:
						shouldExit = true;
						break;
					default:
						break;
				}
			break;
			case SDL_QUIT:
				shouldExit = true;
				break;
			default:
				break;
		}
	}
}

void UpdateTetris(void)
{
	HandleEventsTetris();
	
	currentTime = SDL_GetTicks64();
	
	if (GetTimeSinceLastMove() > speed && CanMoveTetromino(&tetromino, POINT_DOWN, &level))
		MoveTetromino(&tetromino, POINT_DOWN);
	
	if (GetTimeSinceLastMove() > speed * 2)
		PlaceTetromino();
	
	for (int y = 0; y < LEVELHEIGHT; y++)
	{
		if (IsLineFilled(&level, y))
			ClearLine(&level, y);
	}
	
	lastTime = currentTime;
}

void DrawTetris(void)
{
	SDL_SetRenderDrawColor(application.renderer, 0, 0, 0, 255);
	SDL_RenderClear(application.renderer);
	DrawLevel(application.renderer, &level);
	DrawTetromino(application.renderer, &tetromino);
	SDL_RenderPresent(application.renderer);
}

bool ShouldExitTetris(void)
{
	return shouldExit;
}