#pragma once
#include <ncurses.h>

#include <cmath>
#include <iostream>

// It makes sure that x is no less than min and no more than max.
int restrict_value(const int x, const int min, const int max) { return std::max(min, std::min(x, max)); }

// If option is less than _min, option = _max and the other way around.
int restrict_wrap_around(const int x, const int min, const int max)
{
	int toRet = x;

	if (toRet > max)
		toRet = min;
	if (toRet < min)
		toRet = max;

	return toRet;
}

// Returns the number of pages needed to display given number of items (minimum of one).
int get_page_count(const int _num_of_items, const int _single_page_height)
{
	return (std::max(1, (int)std::ceil((double)_num_of_items / (double)_single_page_height)));
}

// Provides a simple way of changing out_selectedOption with arrow keys through input variable.
// Returns true if enter has been pressed and false otherwise.
bool simple_option_cursing(const int input, int &out_selectedOption)
{
	switch (input)
	{
		case KEY_UP:
		case KEY_LEFT:
			--out_selectedOption;
			break;

		case KEY_DOWN:
		case KEY_RIGHT:
			++out_selectedOption;
			break;

		case 10:  // key enter
			return true;
	}
	return false;
}

// Provides a deluxe way of changing out_selectedOption with arrow keys through input variable.
// Changes out_selectedOption increment amount and automatically wraps around out_selectedOption
// with use of min and max parameters or restricts it if wrapAround is set to false.
// Returns true if enter has been pressed and false otherwise.
bool deluxe_option_cursing(const int input, uint32_t &out_selectedOption, const uint32_t increment, const int min,
						   const int max, const bool wrapAround = true)
{
	switch (input)
	{
		case KEY_UP:
		case KEY_LEFT:
			out_selectedOption -= increment;
			break;

		case KEY_DOWN:
		case KEY_RIGHT:
			out_selectedOption += increment;
			break;

		case 10:  // key enter
			return true;

		default:
			return false;
	}

	if (wrapAround)
		out_selectedOption = restrict_wrap_around(out_selectedOption, min, max);
	else
		out_selectedOption = restrict_value(out_selectedOption, min, max);

	return false;
}

// Function makes sure that there are no space, '\n' or '\r' characters at the end of the string.
void delete_end_spaces(std::string &out_str)
{
	while (out_str.size() > 0)  // deleting spaces at the end
	{
		if (out_str.back() == ' ' || out_str.back() == '\n' || out_str.back() == '\r')
			out_str.pop_back();
		else
			break;
	}
}