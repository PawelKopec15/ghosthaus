#pragma once

#include <sstream>

#include "../utility/Vector2.hpp"
#include "Curses.hpp"

class Window
{
public:
	Window(int _height = 8, int _width = 16, int y = 0, int x = 0) : width(_width), height(_height)
	{
		window = newwin(_height, _width, y, x);
	}
	Window(Vector2<int> size, int y = 0, int x = 0) : width(size.x), height(size.y)
	{
		window = newwin(size.y, size.x, y, x);
	}
	~Window() { delwin(window); }

	Window(Window &&)      = delete;
	Window(const Window &) = delete;

	void setScrollLock(bool scrollLock) { scrollok(window, scrollLock); }

	bool move(int y, int x) { return mvwin(window, y, x); }
	bool move(Vector2<int> pos) { return move(pos.y, pos.x); }
	bool moveToX(int x) { return move(getGlobalPositionY(), x); }
	bool moveToY(int y) { return move(y, getGlobalPositionX()); }

	bool moveToBeCenteredByX() { return mvwin(window, getGlobalPositionY(), getCenteredPositionX()); }
	bool moveToBeCenteredByY() { return mvwin(window, getCenteredPositionY(), getGlobalPositionY()); }
	bool moveToBeCentered() { return mvwin(window, getCenteredPositionY(), getCenteredPositionX()); }

	void correctAfterTerminalResizing()
	{
		clear();
		wresize(window, height, width);
	}

	bool resize(int _height, int _width)
	{
		height = _height;
		width  = _width;
		return wresize(window, _height, _width);
	}
	bool resize(Vector2<int> size)
	{
		height = size.y;
		width  = size.x;
		return resize(size.y, size.x);
	}

	Vector2<int> getGlobalPosition()
	{
		auto toRet = Vector2<int>::ZERO();
		getbegyx(window, toRet.y, toRet.x);
		return toRet;
	}
	int getGlobalPositionX() const { return getbegx(window); }
	int getGlobalPositionY() const { return getbegy(window); }

	int getGlobalCenterX() const { return getGlobalPositionX() + (getWidth() / 2); }
	int getGlobalCenterY() const { return getGlobalPositionY() + (getHeight() / 2); }
	Vector2<int> getGlobalCenter() const { return Vector2<int>(getGlobalCenterX(), getGlobalCenterY()); }

	int getCenteredPositionX() const { return Curses::Get().GetTerminalCenterX() - (getWidth() / 2); }
	int getCenteredPositionY() const { return Curses::Get().GetTerminalCenterY() - (getHeight() / 2); }
	Vector2<int> getCenteredPosition() { return Vector2<int>(getCenteredPositionX(), getCenteredPositionY()); }

	void reCreate(int _height = 8, int _width = 16, int y = 0, int x = 0, bool scrollLock = true)
	{
		delwin(window);
		height = _height;
		width  = _width;

		window = newwin(_height, _width, y, x);
		scrollok(window, scrollLock);
	}

	Vector2<int> getSize()
	{
		auto toRet = Vector2<int>::ZERO();
		getmaxyx(window, toRet.y, toRet.x);

		return toRet;
	}
	int getWidth() const { return getmaxx(window); }
	int getHeight() const { return getmaxy(window); }

	int getCenterX() const { return getWidth() / 2; }
	int getCenterY() const { return getHeight() / 2; }
	Vector2<int> getCenter() const { return Vector2<int>(getCenterX(), getCenterY()); }

	void refresh() { wrefresh(window); }
	void clear() { wclear(window); }

	void moveCursor(int y, int x) { wmove(window, y, x); }
	void moveCursor(const Vector2<int> &pos) { wmove(window, pos.y, pos.x); }

	void print(const char *str) { wprintw(window, str); }
	void print(const std::string &str) { wprintw(window, str.c_str()); }
	void print(char c) { waddch(window, c); }
	void printRefresh(char c) { wechochar(window, c); }

	void print(wchar_t wc)
	{
		wchar_t wstr[] = {wc, L'\0'};
		waddwstr(window, wstr);
	}

	void printAt(const std::string &str, int y, int x) { mvwprintw(window, y, x, str.c_str()); }
	void printAt(const char *str, int y, int x) { mvwprintw(window, y, x, str); }
	void printAt(char c, int y, int x) { mvwaddch(window, y, x, c); }

	void printAt(wchar_t wc, int y, int x)
	{
		wchar_t wstr[] = {wc, L'\0'};
		mvwaddwstr(window, y, x, wstr);
	}

	void printAt(const std::string &str, const Vector2<int> &pos) { mvwprintw(window, pos.y, pos.x, str.c_str()); }
	void printAt(const char *str, const Vector2<int> &pos) { mvwprintw(window, pos.y, pos.x, str); }
	void printAt(char c, const Vector2<int> &pos) { mvwaddch(window, pos.y, pos.x, c); }

	void printAt(wchar_t wc, const Vector2<int> &pos) { printAt(wc, pos.y, pos.x); }

	void drawBox(chtype left_right = 0, chtype up_down = 0) { box(window, left_right, up_down); }

	void setColor(short fg, short bg, bool bold = false) const
	{
		wattron(window, COLOR_PAIR(Curses::Get().ColorPairNum(fg, bg)));
		if (bold)
			wattron(window, A_BOLD);
	}

	void unsetColor(short fg, short bg) const
	{
		wattroff(window, COLOR_PAIR(Curses::Get().ColorPairNum(fg, bg)));
		wattroff(window, A_BOLD);
	}

	void erase() const { werase(window); }

private:
	WINDOW *window;
	int width;
	int height;

	std::string wc_to_str(wchar_t wc) const
	{
		std::wstringstream wss;
		wss << wc;

		std::wstring wstr = wss.str();

		// Convert wstring to string
		std::string str(wstr.begin(), wstr.end());

		return str;
	}
};