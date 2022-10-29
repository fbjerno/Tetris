#ifndef TETRIS_POINT
#define TETRIS_POINT

#define POINT_ZERO (Point){0, 0}
#define POINT_RIGHT (Point){1, 0}
#define POINT_LEFT (Point){-1, 0}
#define POINT_UP (Point){0, -1}
#define POINT_DOWN (Point){0, 1}

typedef struct Point
{
	int x;
	int y;
} Point;

Point PointAdd(Point p1, Point p2);

Point PointSubtract(Point p1, Point p2);

Point PointMultiply(Point p1, Point p2);

Point PointDivide(Point p1, Point p2);

Point PointScale(Point p1, int scale);

#endif