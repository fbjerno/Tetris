#ifndef TETRIS_TETROMINO
#define TETRIS_TETROMINO

typedef struct Tetromino Tetromino;

#include "Point.h"
#include "Level.h"
#include "Color.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include <stdbool.h>

typedef enum TetrominoShape { Shape_Start, Shape_T, Shape_I, Shape_L, Shape_J, Shape_S, Shape_Z, Shape_O, Shape_End } TetrominoShape;

struct Tetromino
{
	TetrominoShape shape;
	Point cells[4];
	Color color;
};

TetrominoShape GetRandomTetrominoShape(void);
Tetromino CreateRandomTetromino(Point position);
Tetromino CreateTetromino(Point position, TetrominoShape shape);

void SetTetrominoPosition(Tetromino* tetromino, Point position);
void MoveTetromino(Tetromino* tetromino, Point direction);
bool CanMoveTetromino(const Tetromino* tetromino, Point direction, const Level* level);
bool TryMoveTetromino(Tetromino* tetromino, Point direction, const Level* level);

void RotateTetromino(Tetromino* tetromino);
bool CanRotateTetromino(const Tetromino* tetromino, const Level* level);
bool TryRotateTetromino(Tetromino* tetromino, const Level* level, bool wallKick);

void DrawTetrominoCell(SDL_Renderer* renderer, const SDL_Rect* destRect, Color color);
void DrawTetromino(SDL_Renderer* renderer, const Tetromino* position);
void DrawTetrominoCentered(SDL_Renderer* renderer, const Tetromino* position);

Uint64 GetTimeSinceLastMove(void);
Uint64 GetLastMoveTime(void);

#endif
