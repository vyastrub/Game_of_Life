#include "Cell.hpp"

Cell::Cell(Point const & coord) : _coord(coord), _alive(true)
{
}

Cell::~Cell()
{
}

bool Cell::operator!=(Cell const & cell) const
{
	return (this != &cell);
}

Point & Cell::get_coord() const
{
	return const_cast<Point&>(_coord);
}

void Cell::kill()
{
	_alive = false;
}

bool Cell::cell_is_dead() const
{
	return !_alive;
} 