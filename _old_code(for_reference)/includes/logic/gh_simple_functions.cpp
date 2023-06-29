#include "gh_simple_functions.hpp"

// There is probably a function that does this already in some library but idk so i wrote my own.
// It makes sure that x is no less than _min and no more than _max.
void restrict(int &x, const int _min, const int _max)
{
	x = std::max(_min, x);
	x = std::min(_max, x);
}

// If option is less than _min, option = _max and the other way around.
void restrict_wrap_around(int &option, const int _min, const int _max)
{
	if (option > _max)
		option = _min;
	if (option < _min)
		option = _max;
}

// Returns how many pages are needed to display given number of items (minimum of one).

int get_page_count(const int _num_of_items, const int _single_page_height)
{
	return (std::max(1, (int)std::ceil((double)_num_of_items / (double)_single_page_height)));
}

// Provides a simple way of changing _selected_option with arrow keys through _ch variable.
// Returns 1 if Enter key has been pressed or 0 otherwise.
int simple_option_cursing(const int _ch, int &_selected_option)
{
	switch (_ch)
	{
		case KEY_UP:
		case KEY_LEFT:
			--_selected_option;
			break;

		case KEY_DOWN:
		case KEY_RIGHT:
			++_selected_option;
			break;

		case 10:  // key enter
			return 1;
	}
	return 0;
}

// Function makes sure that there are no space character at the end of the string.
// Returns string without spaces and line brakes at the end.
void delete_end_spaces(std::string &str)
{
	while (str.size() > 0)  // deleting spaces at the end
	{
		if (str.back() == ' ' || str.back() == '\n' || str.back() == '\r')
			str.pop_back();
		else
			break;
	}
}