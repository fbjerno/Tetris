#include "Tetromino.h"
#include "Point.h"
#include "Random.h"
#include "Assets.h"
#include "SDL.h"
#include "SDL_timer.h"
#include "SDL_surface.h"
#include "SDL_image.h"
#include <stdio.h>

SDL_Rect cellRect = { 0, 0, CELLSIZE, CELLSIZE };
SDL_Rect srcRect = {0, 0, CELLSIZE, CELLSIZE};

Uint64 lastMoveTime;

Tetromino CreateRandomTetromino(Point position)
{
	return CreateTetromino(position, (TetrominoShape)RandomInt(Shape_Start + 1, Shape_End - 1));
}

Tetromino CreateTetromino(Point position, TetrominoShape shape)
{
	// cells[0] = rotational origin
	Tetromino tetromino;
	tetromino.shape = shape;
	
	Point* cells = tetromino.cells;
	cells[0].x = 0; cells[0].y = 0;
	
	switch (shape)
	{
		case Shape_T:
			cells[1].x = -1; cells[1].y = 0;
			cells[2].x = 0; cells[2].y = -1;
			cells[3].x = 1; cells[3].y = 0;
			tetromino.color = Color(255, 0, 255, 255);
			break;
		case Shape_I:
			cells[1].x = 1; cells[1].y = 0;
			cells[2].x = 2; cells[2].y = 0;
			cells[3].x = 3; cells[3].y = 0; 
			tetromino.color = Color(50, 225, 200, 255);
			break;
		// Blue L shape
		case Shape_L:
			cells[1].x = -1; cells[1].y = -1;
			cells[2].x = -1; cells[2].y = 0;
			cells[3].x = 1; cells[3].y = 0;
			tetromino.color = Color(0, 0, 255, 255);
			break;
		// Orange J shape (opposite of blue L)
		case Shape_J:
			cells[1].x = -1; cells[1].y = 0;
			cells[2].x = 1; cells[2].y = 0;
			cells[3].x = 1; cells[3].y = -1;
			tetromino.color = Color(255, 150, 0, 255);
			break;
		case Shape_S:
			cells[1].x = -1; cells[1].y = 0;
			cells[2].x = 0; cells[2].y = -1;
			cells[3].x = 1; cells[3].y = -1;
			tetromino.color = Color(0, 255, 0, 255);
			break;
		case Shape_Z:
			cells[1].x = -1; cells[1].y = -1;
			cells[2].x = 0; cells[2].y = -1;
			cells[3].x = 1; cells[3].y = 0;
			tetromino.color = Color(255, 0, 0, 255);
			break;
		case Shape_O:
			cells[1].x = 0; cells[1].y = 1;
			cells[2].x = 1; cells[2].y = 1;
			cells[3].x = 1; cells[3].y = 0;
			tetromino.color = Color(255, 255, 0, 255);
		default:
			break;
	}
	
	SetTetrominoPosition(&tetromino, position);
	
	return tetromino;
}

void SetTetrominoPosition(Tetromino* tetromino, Point position)
{
	Point direction = PointSubtract(position, tetromino->cells[0]);
	MoveTetromino(tetromino, direction);
}

bool CheckTetrominoCollision(const Tetromino* tetromino, const Level* level)
{
	for (int i = 0; i < 4; i++)
	{
		if (IsCellSolid(level, tetromino->cells[i]))
			return true;
	}
	
	return false;
}


void MoveTetromino(Tetromino* tetromino, Point direction)
{
	Point* cells = tetromino->cells;
	for (int i = 0; i < 4; i++)
		cells[i] = PointAdd(cells[i], direction);
	
	lastMoveTime = SDL_GetTicks64();
}

bool CanMoveTetromino(const Tetromino* tetromino, Point direction, const Level* level)
{
	for (int i = 0; i < 4; i++)
	{
		if (IsCellSolid(level, PointAdd(tetromino->cells[i], direction)))
			return false;
	}
	
	return true;
}

bool TryMoveTetromino(Tetromino* tetromino, Point direction, const Level* level)
{
	if (CanMoveTetromino(tetromino, direction, level))
	{
		MoveTetromino(tetromino, direction);
		return true;
	}
	else
	{
		return false;
	}
}

Point RotatePointClockwiseAroundOrigin(Point point, Point origin)
{
	Point relativePos = PointSubtract(point, origin);
	Point rotatedPoint = {-relativePos.y, relativePos.x};
	return PointAdd(rotatedPoint, origin);
}

void RotateTetromino(Tetromino* tetromino)
{
	// Box shape is completely symmetrical so rotation doesn't actually do anything (and origin is in between cells, which is problematic).
	// Bar shape has it's origin in between it's cells, so it needs special rotation logic as this system can't handle decimals
	if (tetromino->shape != Shape_O && tetromino->shape != Shape_I)
	{
		for (int i = 1; i < 4; i++)
			tetromino->cells[i] = RotatePointClockwiseAroundOrigin(tetromino->cells[i], tetromino->cells[0]);
	}
	
	if (tetromino->shape == Shape_I)
	{
		// I can't believe this works
		Point newDirection = RotatePointClockwiseAroundOrigin(PointSubtract(tetromino->cells[1], tetromino->cells[0]), POINT_ZERO);
		tetromino->cells[0] = PointSubtract(tetromino->cells[1], newDirection);
		for (int i = 1; i < 4; i++)
			tetromino->cells[i] = PointAdd(tetromino->cells[0], PointScale(newDirection, i));
	}
}

bool CanRotateTetromino(const Tetromino* tetromino, const Level* level)
{
	Tetromino rotateTest = *tetromino;
	RotateTetromino(&rotateTest);
	
	return !CheckTetrominoCollision(&rotateTest, level);
}

bool TryRotateTetromino(Tetromino* tetromino, const Level* level)
{
	if (CanRotateTetromino(tetromino, level))
	{
		RotateTetromino(tetromino);
		return true;
	}
	else
	{
		return false;
	}
}

void DrawTetrominoCell(SDL_Renderer* renderer, const SDL_Rect* destRect, Color color)
{
	SDL_SetTextureColorMod(assets->tetrominoBase, ColorComponentsRGB(color));
	SDL_RenderCopy(renderer, assets->tetrominoBase, &srcRect, destRect);
	
	SDL_SetTextureColorMod(assets->tetrominoBase, 255, 255, 255);
	SDL_RenderCopy(renderer, assets->tetrominoHighlight, &srcRect, destRect);
}

void DrawTetromino(SDL_Renderer* renderer, const Tetromino* tetromino)
{
	for (int i = 0; i < 4; i++)
	{
		cellRect.x = tetromino->cells[i].x * CELLSIZE;
		cellRect.y = tetromino->cells[i].y * CELLSIZE;
		DrawTetrominoCell(renderer, &cellRect, tetromino->color);
	}
}

Uint64 GetTimeSinceLastMove(void)
{
	return SDL_GetTicks64() - lastMoveTime;
}

Uint64 GetLastMoveTime(void)
{
	return lastMoveTime;
}