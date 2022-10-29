#ifndef TETRIS_LEVEL
#define TETRIS_LEVEL

#define LEVELWIDTH 10
#define LEVELHEIGHT 20
#define LEVELSIZE LEVELWIDTH * LEVELHEIGHT
#define CELLSIZE 16

typedef struct Level Level;

#include "Point.h"
#include "Tetromino.h"
#include "SDL_stdinc.h"
#include "SDL_render.h"
#include <stdbool.h>

struct Level
{
	// Placeholder
	Sint32 map[LEVELWIDTH * LEVELHEIGHT];
};

void DrawLevel(SDL_Renderer* renderer, const Level* level);

int IsCellSolid(const Level* level, Point position);

// Includes roof
int InMapBounds(Point position);
// Excludes roof
int InLevelBounds(Point position);

bool IsLineFilled(const Level* level, int y);

void ClearLine(Level* level, int y);

void PlaceTetrominoInLevel(Level* level, Tetromino* tetromino);

#endif