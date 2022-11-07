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

TetrominoShape GetRandomTetrominoShape(void)
{
    return (TetrominoShape)RandomInt(Shape_Start + 1, Shape_End - 1);
}

Tetromino CreateRandomTetromino(Point position)
{
  return CreateTetromino(position, GetRandomTetrominoShape());
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
			cells[3].x = -1; cells[3].y = 0; 
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
	Point* cells = tetromino->cells;
	// Box shape is completely symmetrical so rotation doesn't actually do anything (and origin is in between cells, which is problematic).
	// Bar shape has it's origin in between it's cells, so it needs special rotation logic as this system can't handle decimals
	if (tetromino->shape != Shape_O && tetromino->shape != Shape_I)
	{
		for (int i = 1; i < 4; i++)
			cells[i] = RotatePointClockwiseAroundOrigin(cells[i], cells[0]);
	}
	
	if (tetromino->shape == Shape_I)
	{	
		Point newDirection = RotatePointClockwiseAroundOrigin(PointSubtract(tetromino->cells[1], tetromino->cells[0]), POINT_ZERO);
		// Since cells[0] is considered origin and is the second cell in the bar (closer to center), we have to do this nonsense.
		// If the origin was the first cell in the bar, you could instead write for(int i = 1...) cells[i] = cells[0] + direction * i, which is much more clear
		cells[0] = tetromino->cells[1];
		cells[1] = PointAdd(cells[0], newDirection);
		cells[2] = PointAdd(cells[1], newDirection);
		cells[3] = PointSubtract(cells[0], newDirection);
	}
}

bool CanRotateTetromino(const Tetromino* tetromino, const Level* level)
{
	Tetromino rotateTest = *tetromino;
	RotateTetromino(&rotateTest);
	return !CheckTetrominoCollision(&rotateTest, level);
}

bool TryRotateTetromino(Tetromino* tetromino, const Level* level, bool wallKick)
{
	static const Point offsets[7] = {{0, 0}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {2, 0}, {-2, 0}};
	Tetromino rotateTest = *tetromino;
	RotateTetromino(&rotateTest);
	Point originalPosition = rotateTest.cells[0];
	
	int count = rotateTest.shape == Shape_I ? 7 : 5;
	for (int i = 0; i < count; i++)
	{
		SetTetrominoPosition(&rotateTest, PointAdd(originalPosition, offsets[i]));
		if (!CheckTetrominoCollision(&rotateTest, level))
		{
			*tetromino = rotateTest;
			return true;
		}
	}
	
	return false;
}

void DrawTetrominoCell(SDL_Renderer* renderer, const SDL_Rect* destRect, Color color)
{
	SDL_SetTextureColorMod(assets.tetrominoBase, ColorComponentsRGB(color));
	SDL_RenderCopy(renderer, assets.tetrominoBase, &srcRect, destRect);
	
	SDL_SetTextureColorMod(assets.tetrominoBase, 255, 255, 255);
	SDL_RenderCopy(renderer, assets.tetrominoHighlight, &srcRect, destRect);
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

void DrawTetrominoOutline(SDL_Renderer* renderer, const Tetromino* tetromino, SDL_Color color)
{
	// Using inline instead of outline because it looks better
	static SDL_Rect inlineRect = {0, 0, CELLSIZE - 2, CELLSIZE - 2};
	for (int i = 0; i < 4; i++)
	{
		inlineRect.x = tetromino->cells[i].x * CELLSIZE;
		inlineRect.y = tetromino->cells[i].y * CELLSIZE;
		SDL_SetRenderDrawColor(renderer, 75, 75, 75, 255);//ColorComponents(tetromino->color));
		SDL_RenderDrawRect(renderer, &inlineRect);
	}
}

void DrawTetrominoCentered(SDL_Renderer* renderer, const Tetromino* tetromino)
{
  	for (int i = 0; i < 4; i++)
  	{
		cellRect.x = tetromino->cells[i].x * CELLSIZE;
		cellRect.y = tetromino->cells[i].y * CELLSIZE;
		if (tetromino->shape == Shape_O)
		{
			cellRect.x -= CELLSIZE / 2;
			cellRect.y -= CELLSIZE;
		}
		else if (tetromino->shape == Shape_I)
		{
			cellRect.x -= CELLSIZE / 2;
			cellRect.y -= CELLSIZE / 2;
		}
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
