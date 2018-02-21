#pragma once
#define NCURSES_MOUSE_VERSION

#include <list>
#include <windows.h>
#include "Cell.hpp"
#include "curses.h"

class Game_loop
{
public:
	Game_loop();
	~Game_loop();

	void render();      /**/
	void game_over();	/* Visualisation	*/
	void input_menu();	/**/

	/*	----	logic part	 ----	*/	
	bool is_alive(Cell const & current_cell) const;		// Check if cell still alive on next generation
	void push_point(int x, int y);						/*-----------------------------------------*/
	void collect_points(Cell const & current_cell);		/*Add empty point to potentional cell stack*/
	void update_population();							/*Remowing dead cells and add borned*/

	void init_cells();  
	void run();

private:

	int height;
	int width;

	std::list<Point>					input;  
	std::list<Cell>						cells;
	std::list<std::pair<int, Point>>	point_map;
};	

