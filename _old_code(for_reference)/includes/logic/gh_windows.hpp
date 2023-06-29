#pragma once

#include <ncurses.h>

#include <cmath>
#include <iostream>
#include <vector>

#include "../singleton/gh_singleton_camera.hpp"
#include "gh_constants_and_colors.hpp"

using std::vector;

// Saves terminal dimensions into given variables
// If terminal is smaller than TERM_MIN_WIDTH x TERM_MIN_HEIGHT it returns 1
int get_terminal_dimensions(int &terminal_w, int &terminal_h);

// Sets up windows for episode select
int setup_episode_select_windows(WINDOW *&win_adventure_select, const int terminal_w, const int terminal_h);

// Sets up windows for title screen
int setup_title_screen_windows(WINDOW *&win_logo, WINDOW *&win_options, const int terminal_w, const int terminal_h);

// Sets up all necessary WINDOW* variables for the action of the game
// makes use of many #define directives
int setup_action_windows(const int terminal_w, const int terminal_h, int &margin_side, int &margin_topbottom);
