#include "Tetris.h"
#include "Application.h"
#include "Point.h"
#include "Level.h"
#include "Tetromino.h"
#include "Input.h"
#include "Assets.h"
#include "SDL.h"
#include "SDL_timer.h"
#include "SDL_events.h"
#include "SDL_keyboard.h"
#include "SDL_keycode.h"
#include "SDL_FontCache.h"
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define START_SPEED 500
#define MAX_SPEED 50
#define TETROMINO_START (Point){LEVELWIDTH / 2 - 1, 0}

#define TETROMINO_PREVIEWPOS (Point){3, 7}

const SDL_Rect gameBounds = {128, 0, LEVELWIDTH * CELLSIZE, LEVELHEIGHT * CELLSIZE};
const SDL_Rect leftPanelBounds = {0, 0, 128, gameBounds.h};
const SDL_Rect rightPanelBounds = {gameBounds.x + gameBounds.w, 0, 128, gameBounds.h};
const SDL_Rect tetrominoPreviewBackgroundRect = {CELLSIZE, 5 * CELLSIZE, 5 * CELLSIZE, 4 * CELLSIZE};
const SDL_Rect tetrominoPreviewForegroundRect = {CELLSIZE + CELLSIZE / 4, 5 * CELLSIZE + CELLSIZE / 4, 5 * CELLSIZE - CELLSIZE / 2, 4 * CELLSIZE - CELLSIZE / 2};

int speed = 500;
int tetrominosPlaced = 0;
int points = 0;

Level level;
Tetromino tetromino;

Uint64 currentTime = 0;
Uint64 lastTime = 0;

Uint64 lastMoveYTime = 0;
Uint64 lastMoveXTime = 0;

bool shouldExit = false;

Tetromino nextTetromino;
Tetromino storedTetromino = {0};
bool hasStoredTetromino = false;

FC_Effect textEffect;

TetrisInput lastInput;

Tetromino GetNextTetromino(void)
{
	Tetromino next = nextTetromino;
	SetTetrominoPosition(&next, TETROMINO_START);
	nextTetromino = CreateRandomTetromino(TETROMINO_PREVIEWPOS);
	return next;
}

void GetStoredTetromino(void)
{
	Tetromino stored = storedTetromino;
	storedTetromino = CreateTetromino(TETROMINO_PREVIEWPOS, tetromino.shape);

	if (stored.shape == 0)
	{
		tetromino = GetNextTetromino();
	}
	else
	{
		tetromino = stored;
		SetTetrominoPosition(&tetromino, TETROMINO_START);
	}
  
  hasStoredTetromino = true;
}

void PlaceTetromino(void)
{
    PlaceTetrominoInLevel(&level, &tetromino);
    tetromino = GetNextTetromino();
    tetrominosPlaced++;
    
    speed = START_SPEED - 1 * (int)logf((float)tetrominosPlaced);
    if (speed < MAX_SPEED)
      speed = MAX_SPEED;

    hasStoredTetromino = false;
}

void InitializeTetris(void)
{
	memset(level.map, 0, sizeof(level.map));
	nextTetromino = CreateRandomTetromino((Point){0, 4});
	tetromino = GetNextTetromino();
	textEffect = FC_MakeEffect(FC_ALIGN_CENTER, FC_MakeScale(1.0f, 1.0f), FC_MakeColor(220, 20, 20, 255));
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
				/*
					case SDLK_LEFT:
						TryMoveTetromino(&tetromino, POINT_LEFT, &level);
						break;
					case SDLK_RIGHT:
						TryMoveTetromino(&tetromino, POINT_RIGHT, &level);
						break;
					case SDLK_DOWN:
						TryMoveTetromino(&tetromino, POINT_DOWN, &level);
						break;
					*/
					case SDLK_UP:
						// TODO: only rotate when clicked, not when held
						TryRotateTetromino(&tetromino, &level, true);
						break;
					case SDLK_SPACE:
						PlaceTetromino();
						break;
					case SDLK_ESCAPE:
						shouldExit = true;
						break;
				        case SDLK_LSHIFT:
					    if (!hasStoredTetromino)
							GetStoredTetromino();
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

	TetrisInput input = InputFromKeyboardState(SDL_GetKeyboardState(NULL));
	if (currentTime - lastMoveXTime > 70)
	{
		int moveX = 0;
		if (input.left)
			moveX--;
		if (input.right)
			moveX++;
		if (moveX != 0)
		{
			if(TryMoveTetromino(&tetromino, (Point){moveX, 0}, &level))
				lastMoveXTime = currentTime;
		}
	}
	
	if (currentTime - lastMoveYTime > 70)
	{
		if (input.down)
		{
			if (TryMoveTetromino(&tetromino, POINT_DOWN, &level))
				lastMoveYTime = currentTime;
		}
	}
}

void UpdateTetris(void)
{
	currentTime = SDL_GetTicks64();
	
    HandleEventsTetris();
	
	if (currentTime - lastMoveYTime > speed)
	{
		if (!TryMoveTetromino(&tetromino, POINT_DOWN, &level))
			PlaceTetromino();
			
		lastMoveYTime = currentTime;
	}
	
	int linesCleared = 0;
	for (int y = 0; y < LEVELHEIGHT; y++)
    {
        if (IsLineFilled(&level, y))
        {     
            ClearLine(&level, y);
            linesCleared++;
        }
	}

	points += linesCleared * linesCleared;
	
	lastTime = currentTime;

    hasStoredTetromino = false;
}

void DrawGame(void)
{
    SDL_RenderSetViewport(application.renderer, &gameBounds);
    DrawLevel(application.renderer, &level);
    DrawTetromino(application.renderer, &tetromino);
}

void DrawLeftPanel(void)
{
	SDL_Rect origin = leftPanelBounds;
	origin.x += CELLSIZE / 2;
	SDL_RenderSetViewport(application.renderer, &origin);
	
	SDL_SetRenderDrawColor(application.renderer, 20, 20, 120, 255);
	SDL_RenderFillRect(application.renderer, &tetrominoPreviewBackgroundRect);
	SDL_SetRenderDrawColor(application.renderer, 20, 20, 60, 255); 	
	SDL_RenderFillRect(application.renderer, &tetrominoPreviewForegroundRect);
	
	if (storedTetromino.shape != 0)
		DrawTetrominoCentered(application.renderer, &storedTetromino);
	
	SDL_RenderSetViewport(application.renderer, &leftPanelBounds);
	
	FC_DrawEffect(assets.titleFont, application.renderer, (float)leftPanelBounds.w * 0.5f, (float)leftPanelBounds.h * 0.1f, textEffect, "TETRIS");
	FC_DrawEffect(assets.textFont, application.renderer, (float)leftPanelBounds.w * 0.5f, (float)leftPanelBounds.h * 0.5f, textEffect, "%i", points * 100);
}

void DrawRightPanel(void)
{
	// hack to center tetromino
	SDL_Rect origin = rightPanelBounds;
	origin.x += CELLSIZE / 2;
	SDL_RenderSetViewport(application.renderer, &origin);
	SDL_SetRenderDrawColor(application.renderer, 20, 20, 120, 255);
	SDL_RenderFillRect(application.renderer, &tetrominoPreviewBackgroundRect);
	SDL_SetRenderDrawColor(application.renderer, 20, 20, 60, 255);
	SDL_RenderFillRect(application.renderer, &tetrominoPreviewForegroundRect);
	DrawTetrominoCentered(application.renderer, &nextTetromino);

	SDL_RenderSetViewport(application.renderer, &origin);
	FC_DrawEffect(assets.textFont, application.renderer, (float)(tetrominoPreviewForegroundRect.x + tetrominoPreviewForegroundRect.w / 2), (float)(tetrominoPreviewForegroundRect.y - 32), textEffect, "NEXT");
}

void DrawTetris(void)
{
    SDL_SetRenderDrawColor(application.renderer, 0, 0, 0, 255);
    SDL_RenderClear(application.renderer);
    DrawLeftPanel();
    DrawGame();
    DrawRightPanel();
    SDL_RenderPresent(application.renderer);
}

bool ShouldExitTetris(void)
{
    return shouldExit;
}
