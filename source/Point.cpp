#include "point.hpp"

Point::Point(int _x, int _y) : x(_x), y(_y)
{
}

bool Point::operator<=(const Point & coord) const
{
	if (coord.x > x - 2 && coord.x < x + 2 &&
		coord.y > y - 2 && coord.y < y + 2)
		return true;
	else
		return false;
}

bool Point::operator==(const Point & coord) const
{
	return (coord.x == x && coord.y == y);
}

bool Point::operator!=(const Point & coord) const
{
	return (coord.x != x && coord.y != y);
}

bool Point::out_of_map(int x0, int y0, int width, int height)
{
	return (x < x0 || y < y0 || x >= width || y >= height);
}