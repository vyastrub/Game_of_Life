#pragma once

struct Point
{
	int x;
	int y;

	Point(int _x, int _y);
	bool operator==(const Point & coord) const;
	bool operator!=(const Point & coord) const;

	bool operator<=(const Point & coord) const; //Overloar to check if the point is neighbour
	bool out_of_map(int x0, int y0, int width, int height);
};