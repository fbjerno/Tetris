#include "Level.h"
#include "Color.h"
#include "SDL_render.h"
#include "SDL.h"
#include <string.h>

void DrawLevel(SDL_Renderer* renderer, const Level* level)
{
	SDL_Rect levelBounds = {0, 0, LEVELWIDTH * CELLSIZE, LEVELHEIGHT * CELLSIZE};
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &levelBounds);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &levelBounds);
	
	SDL_Rect cellRect = {0, 0, CELLSIZE, CELLSIZE};
	
	for (int i = 0; i < LEVELSIZE; i++)
	{
		if (level->map[i])
		{
			cellRect.x = i % LEVELWIDTH * CELLSIZE;
			cellRect.y = i / LEVELWIDTH * CELLSIZE;
			Color color = *(Color*)(level->map + i);
			
			DrawTetrominoCell(renderer, &cellRect, color);
		}
	}
}

int IsCellSolid(const Level* level, Point position)
{
	if (InMapBounds(position))
	{
		return level->map[position.x + position.y * LEVELWIDTH];
	}
	else
	{
		return !InLevelBounds(position);
	}
}

inline int InMapBounds(Point position)
{
	return position.x >= 0 && position.x < LEVELWIDTH && position.y >= 0 && position.y < LEVELHEIGHT;
}

inline int InLevelBounds(Point position)
{
	return position.x >= 0 && position.x < LEVELWIDTH && position.y < LEVELHEIGHT;
}

bool IsLineFilled(const Level* level, int y)
{
	for (int x = 0; x < LEVELWIDTH; x++)
	{
		if (!level->map[x + y * LEVELWIDTH])
			return false;
	}
	
	return true;
}

void ClearLine(Level* level, int y)
{
	memmove(level->map + LEVELWIDTH, level->map, (y) * LEVELWIDTH * sizeof(Sint32));
}

void PlaceTetrominoInLevel(Level* level, Tetromino* tetromino)
{
	while (CanMoveTetromino(tetromino, POINT_DOWN, level))
		MoveTetromino(tetromino, POINT_DOWN);
	
	int* map = level->map;
	Point* cells = tetromino->cells;
	for (int i = 0; i < 4; i++)
	{
		Point position = cells[i];
		if (InMapBounds(position))
			map[position.x + position.y * LEVELWIDTH] = *((int*)(&tetromino->color));
	}
}