#pragma once

#include <curses.h>

#include <iostream>

#include "../utility/Vector2.hpp"

class Curses
{
public:
	static Curses &Get()
	{
		static Curses INSTANCE;
		return INSTANCE;
	}
	Curses(Curses &&)      = delete;
	Curses(const Curses &) = delete;
	Curses &operator=(Curses &&) = delete;
	Curses &operator=(const Curses &) = delete;

	~Curses() { EndWin(); }

	void Refresh() const { refresh(); }
	void RefreshScreen() const { Refresh(); }

	void Clear() const { clear(); }

	void EndWin() const { endwin(); }

	Vector2<int> GetTerminalSize() const
	{
		auto toRet = Vector2<int>::ZERO();
		getmaxyx(stdscr, toRet.y, toRet.x);

		return toRet;
	}
	Vector2<int> GetTerminalCenter() const { return GetTerminalSize() / 2; }

	int GetTerminalWidth() const { return getmaxx(stdscr); }
	int GetTerminalHeight() const { return getmaxy(stdscr); }

	int GetTerminalCenterX() const { return getmaxx(stdscr) / 2; }
	int GetTerminalCenterY() const { return getmaxy(stdscr) / 2; }

	int GetCh() const { return getch(); }
	int GetChFlushInp() const
	{
		int toRet = getch();
		flushinp();
		return toRet;
	}

	void MoveCursor(int y, int x) const { move(y, x); }
	void MoveCursor(const Vector2<int> &pos) const { move(pos.y, pos.x); }
	Vector2<int> GetCursorPosition()
	{
		auto toRet = Vector2<int>::ZERO();
		getyx(stdscr, toRet.y, toRet.x);
		return toRet;
	}

	void Print(const char *str) const { printw(str); }
	void Print(const std::string &str) const { printw(str.c_str()); }
	void Print(char c) const { addch(c); }
	void PrintRefresh(char c) const { echochar(c); }

	void Print(wchar_t wc) const
	{
		wchar_t wstr[] = {wc, L'\0'};
		addwstr(wstr);
	}

	void PrintAt(const std::string &str, int y, int x) const { mvprintw(y, x, str.c_str()); }
	void PrintAt(const char *str, int y, int x) const { mvprintw(y, x, str); }
	void PrintAt(char c, int y, int x) const { mvaddch(y, x, c); }

	void PrintAt(wchar_t wc, int y, int x) const
	{
		wchar_t wstr[] = {wc, L'\0'};
		mvaddwstr(y, x, wstr);
	}

	void PrintAt(const std::string &str, const Vector2<int> &pos) const { mvprintw(pos.y, pos.x, str.c_str()); }
	void PrintAt(const char *str, const Vector2<int> &pos) const { mvprintw(pos.y, pos.x, str); }
	void PrintAt(char c, const Vector2<int> &pos) const { mvaddch(pos.y, pos.x, c); }

	void PrintAt(wchar_t wc, const Vector2<int> &pos) const { PrintAt(wc, pos.y, pos.x); }

	void BoxScreen(chtype left_right = 0, chtype up_down = 0) const { box(stdscr, left_right, up_down); }

	short ColorPairNum(short fg, short bg) const { return 16 * fg + bg; }

	void SetColor(short fg, short bg, bool bold = false) const
	{
		attron(COLOR_PAIR(ColorPairNum(fg, bg)));
		if (bold)
			attron(A_BOLD);
	}

	void UnsetColor(short fg, short bg) const
	{
		attroff(COLOR_PAIR(ColorPairNum(fg, bg)));
		attroff(A_BOLD);
	}

	void Erase() const { erase(); }

	void Init()
	{
		if (is_setup)
			return;

		// Setting up locale
		if (setlocale(LC_ALL, "") == nullptr)
			std::cerr << std::endl << std::endl << "Unable to setlocale: game will not display properly." << std::endl;

		// Setting up ncurses
		initscr();

		ReInit();

		if (!has_colors())
		{
			Print("It seems that your terminal does not support color.\nA terminal with color is recommended.");
			getch();
		}
		else
			start_color();

		init_color_pairs();

		is_setup = true;
	}

	void ReInit() const
	{
		noecho();
		cbreak();

		refresh();
		curs_set(1);
		refresh();
		curs_set(0);

		keypad(stdscr, TRUE);
		nodelay(stdscr, TRUE);
	}

private:
	bool is_setup = false;

	Curses() { Init(); }

	void init_color_pairs() const
	{
		for (short bg = 0; bg < 8; ++bg)
		{
			for (short fg = 0; fg < 8; ++fg)
			{
				init_pair(ColorPairNum(fg, bg), fg, bg);
			}
		}
	}
};