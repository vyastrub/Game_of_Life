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
					for (auto i: input)
						if (i == tmp)
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
	for (auto i: cells)
		mvaddch(i.get_coord().y, i.get_coord().x, ' ');
	attroff(A_REVERSE);
	refresh();
	Sleep(1000);
}

void Game_loop::game_over()
{
	int c;

	input.clear();
	cells.clear();
	clear();                                                 
	mvprintw(height / 2 - 3, width / 2 - 28, "  ____    _    __  __ _____    _____     _______ ____   ");
	mvprintw(height / 2 - 2, width / 2 - 28, " / ___|  / \\  |  \\/  | ____|  / _ \\ \\   / | ____|  _ \\  ");
	mvprintw(height / 2 - 1, width / 2 - 28, "| |  _  / _ \\ | |\\/| |  _|   | | | \\ \\ / /|  _| | |_) | ");
	mvprintw(height / 2,     width / 2 - 28, "| |_| |/ ___ \\| |  | | |___  | |_| |\\ V / | |___|  _ <  ");
	mvprintw(height / 2 + 1, width / 2 - 28, " \\____/_/   \\_|_|  |_|_____|  \\___/  \\_/  |_____|_| \\_\\ ");                                                   
	mvprintw(height / 2 + 2, width / 2 - 12, "Press Enter to try again");
	mvprintw(height / 2 + 3, width / 2 - 9, "Press Esc to EXIT");
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
	for (auto  & i: cells)
	{
		if (i != current_cell)
			if (current_cell.get_coord() <= i.get_coord())
				if (!(i.get_coord().out_of_map(0, 0, width, height)))
					neighbours++;
	}
	if (neighbours == 2 || neighbours == 3)
		return true;
	else
		return false;
}

void Game_loop::find_dead()
{
	for (auto & i : cells)
		if (!is_alive(i))
			i.kill();
}

void Game_loop::push_point(int x, int y)
{
	Point tmp(x, y);

	for (auto & i: cells)
	{
		if (i.get_coord() == tmp)
			return;
	}
	
	for (auto & i : point_map)
	{
		if (i.second == tmp)
		{
			i.first++;
			return;
		}
	}
	point_map.push_back(std::make_pair(1, tmp));
}

void Game_loop::collect_points()
{
	for (auto & i : cells)
	{
		int x = i.get_coord().x;
		int y = i.get_coord().y;

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
}


void Game_loop::update_population()
{
	/* ----- Deleting old cells ----- */
	for (auto it = cells.begin(); it != cells.end(); )
	{
		if ((*it).cell_is_dead())
			cells.erase(it++ );  
		else
			++it;
	}

	/* ----- Creating new cells ----- */
	for (auto i : point_map)
	{
		if (i.first == 3)
			cells.push_back(i.second);
	}
	point_map.clear();
}


void Game_loop::init_cells()
{
	for (auto & i : input)
		cells.push_back(i);
	input.clear();
}

void Game_loop::run()
{
	curs_set(0);
	input_menu();
	init_cells();

	while (!cells.empty())
	{
		std::thread t1(&Game_loop::render, this);			//rendering
		std::thread t2(&Game_loop::collect_points, this);	//searching new cells
		std::thread t3(&Game_loop::find_dead, this);		//searching dead cells
		t1.join();
		t2.join();
		t3.join();
		update_population();
		if (getch() == 27)
			break;
	}
	game_over();
}