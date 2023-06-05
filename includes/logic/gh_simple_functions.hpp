#pragma once

#include <ncurses.h>

#include <cmath>
#include <iostream>


// There is probably a function that does this already in some library but idk
// so i wrote my own. It makes sure that x is no less than _min and no more than
// _max.
void restrict(int &x, const int _min, const int _max);

// If option is less than _min, option = _max and the other way around.
void restrict_wrap_around(int &option, const int _min, const int _max);

// Returns how many pages are needed to display given number of items. (minimum
// of one)
int get_page_count(const int _num_of_items, const int _single_page_height);

// Provides a simple way of changing _selected_option with arrow keys through
// _ch variable. Returns 1 if Enter key has been pressed or 0 otherwise.
int simple_option_cursing(const int _ch, int &_selected_option);

// Returns string without spaces at the end
void delete_end_spaces(std::string& str);