#pragma once
#include "point.hpp"

class Cell
{
public:
	Cell(Point const & coord);
	~Cell();
	Point & get_coord() const;
	void kill();
	bool cell_is_dead() const;
	bool operator!=(Cell const & cell) const;

private:
	Point	_coord;
	bool	_alive;
};

