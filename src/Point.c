#include "Point.h"

Point PointAdd(Point p1, Point p2)
{
	Point result = {p1.x + p2.x, p1.y + p2.y};
	return result;
}

Point PointSubtract(Point p1, Point p2)
{
	Point result = {p1.x - p2.x, p1.y - p2.y};
	return result;
}

Point PointMultiply(Point p1, Point p2)
{
	Point result = {p1.x * p2.x, p1.y * p2.y};
	return result;
}

Point PointDivide(Point p1, Point p2)
{
	Point result = {p1.x / p2.x, p1.y / p2.y};
	return result;
}


Point PointScale(Point p1, int scale)
{
	Point result = {p1.x * scale, p1.y * scale};
	return result;
}