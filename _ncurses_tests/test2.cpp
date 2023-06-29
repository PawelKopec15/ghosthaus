#include <iostream>

#include "ncurses.h"

int main(int argc, char const* argv[])
{
	initscr();
	cbreak();
	noecho();
	setlocale(LC_ALL, "");

	WINDOW* win1 = newwin(10, 20, 10, 10);
	WINDOW* win2 = newwin(10, 20, 10, 40);

	scrollok(win1, false);
	scrollok(win2, true);

	box(win1, 0, 0);
	box(win2, 0, 0);

	refresh();

	for (int i = 0;; ++i)
	{
		wrefresh(win1);
		wrefresh(win2);

		switch (i % 3)
		{
			default:
				wprintw(win1, "S");
				wprintw(win2, "S");
				break;

			case 1:
				waddwstr(win1, L"█");
				waddwstr(win2, L"█");
				break;
			case 2:
				waddch(win1, 'c');
				waddch(win2, 'c');
				break;
		}

		getch();
	}

	endwin();

	return 0;
}
