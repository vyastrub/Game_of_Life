#include "Game_loop.hpp"

Game_loop::Game_loop()
{
	initscr();
	clear();
	noecho();
	cbreak();
	curs_set(0);
	keypad(stdscr, TRUE);
	mousemask(ALL_MOUSE_EVENTS, NULL);
	nodelay(stdscr, TRUE);
	getmaxyx(stdscr, height, width);
}


Game_loop::~Game_loop()
{
	endwin();
}


void Game_loop::input_menu()
{
	MEVENT event1;
	std::list<Point>::iterator it;
	bool unique;

	clear();
	mvprintw(height / 2, width / 2 - 18, "Set cells positions and press Enter");
	mvprintw(height - 1, 0, "[ESC]-> Menu");
	refresh();
	Sleep(3000);
	clear();
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			mvaddch(i, j, ACS_BULLET);
	refresh();
	while (1)
	{
		unique = true;
		int c = wgetch(stdscr);
		if (c == KEY_MOUSE)
		{
			if (getmouse(&event1) == OK)
			{
				if (event1.bstate && BUTTON1_PRESSED)
				{
					Point tmp(event1.x, event1.y);
					for (it = input.begin(); it != input.end(); it++)
						if ((*it) == tmp)
							unique = false;
					if (unique)
					{
						mvaddch(event1.y, event1.x, 'X');
						input.push_back(tmp);
						refresh();
					}
				}
			}
		}
		else if (c == 10)
			break;
	}
}

void Game_loop::render()
{
	clear();
	attron(A_REVERSE);
	std::list<Cell>::iterator it;
	for (it = cells.begin(); it != cells.end(); it++)
		mvaddch((*it).get_coord().y, (*it).get_coord().x, ' ');
	attroff(A_REVERSE);
	refresh();
}

void Game_loop::game_over()
{
	int c;

	clear();
	mvprintw(height / 2 - 1, width / 2 - 5, "GAME OVER");
	mvprintw(height / 2, width / 2 - 12, "Press Enter to try again");
	mvprintw(height / 2 + 1, width / 2 - 9, "Press Esc to EXIT");
	refresh();

	while (1)
	{
		c = wgetch(stdscr);
		if (c == 27)
		{
			endwin();
			exit(0);
		}
		else if (c == 10)
			run();
	}
}

bool Game_loop::is_alive(Cell const & current_cell) const
{
	if (current_cell.get_coord().out_of_map(0, 0, width, height))
		return false;

	int neighbours = 0;
	std::list<Cell>::const_iterator it = cells.begin();
	while (it != cells.end())
	{
		if (*it != current_cell)
			if (current_cell.get_coord() <= (*it).get_coord())
				if (!((*it).get_coord().out_of_map(0, 0, width, height)))
					neighbours++;
		it++;
	}
	if (neighbours == 2 || neighbours == 3)
		return true;
	else
		return false;
}

void Game_loop::push_point(int x, int y)
{
	Point tmp(x, y);

	std::list<Cell>::iterator it = cells.begin();
	while (it != cells.end())
	{
		if ((*it).get_coord() == tmp)
			return;
		it++;
	}

	for (int i = 0; i < size; i++)
	{
		if (to_add[i] == tmp)
		{
			point_occasions[i]++;
			return;
		}
	}
	to_add.push_back(tmp);
	point_occasions.push_back(1);
	size++;
}

void Game_loop::collect_points(Cell const & current_cell)
{
	int x = current_cell.get_coord().x;
	int y = current_cell.get_coord().y;

	if (x > 0)
		push_point(x - 1, y);

	if (x < width - 1)
		push_point(x + 1, y);

	if (y > 0)
	{
		push_point(x, y - 1);
		if (x > 0)
			push_point(x - 1, y - 1);
		if (x < width - 1)
			push_point(x + 1, y - 1);
	}

	if (y < height - 1)
	{
		push_point(x, y + 1);
		if (x > 0)
			push_point(x - 1, y + 1);
		if (x < width - 1)
			push_point(x + 1, y + 1);
	}
}

void Game_loop::update_population()
{
	std::list<Cell>::iterator it;

	/*--- Searching for dead and borned cells --- */
	for (it = cells.begin(); it != cells.end(); it++)
	{
		collect_points(*it);
		if (!is_alive(*it))
			(*it).kill();
	}

	/* ----- Deleting old cells ----- */
	for (it = cells.begin(); it != cells.end(); )
	{
		if ((*it).cell_is_dead())
			cells.erase(it++);
		else
			it++;
	}

	/* ----- Creating new cells ----- */
	for (int i = 0; i < size; i++)
	{
		if (point_occasions[i] == 3)
			cells.push_back(to_add[i]);
	}

	to_add.clear();
	point_occasions.clear();
	size = 0;
}


void Game_loop::init_cells()
{
	size = 0;

	std::list<Point>::const_iterator it = input.begin();
	while (it != input.end())
	{
		cells.push_back(*it);
		it++;
	}
	input.clear();
}

void Game_loop::run()
{
	int c;

	curs_set(0);
	input_menu();
	init_cells();

	while (!cells.empty())
	{
		getmaxyx(stdscr, height, width);
		render();
		Sleep(1000);
		update_population();
		c = getch();
		if (c == 27)
		{
			input.clear();
			cells.clear();
			game_over();
		}
	}
	game_over();
}