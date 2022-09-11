#pragma once

#include <iostream>
#include <ncurses.h>
#include <locale.h>
#include <vector>
#include <math.h>

#include "gh_constants_and_colors.h"

using std::vector;

// Saves terminal dimentions into given variables
// If terminal is smaller than TERM_MIN_WIDTH x TERM_MIN_HEIGHT it returns 1
int get_terminal_dimentions(int &terminal_w, int &terminal_h);

// Sets up windows for episode select
int setup_episode_select_windows(WINDOW *&win_adventure_select, const int terminal_w, const int terminal_h);

// Sets up windows for title screen
int setup_title_screen_windows(WINDOW *&win_logo, WINDOW *&win_options, const int terminal_w, const int terminal_h);

// Sets up all nessecary WINDOW* variables for the action of the game
// makes use of many #define directives
int setup_action_windows(WINDOW *&win_messages, WINDOW *&win_inventory, WINDOW *&win_main_border, WINDOW *&win_debug, vector<vector<WINDOW *>> &win_squares, const int terminal_w, const int terminal_h, int &squares_h, int &squares_v, int &margin_side, int &margin_topbottom);
