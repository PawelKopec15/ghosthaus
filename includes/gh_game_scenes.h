#pragma once

#include <ncurses.h>
#include <iostream>
#include <vector>

#include "gh_simple_functions.h"
#include "gh_game_routines.h"
#include "gh_game_logic.h"

using std::vector;

// Creates all windows for title screen, has all the title screen logic and destroys windows at the end
// returns 0 if option to start a new game has been selected
// returns 1 if terminal is smaller than 80x24 or game should end
int scene_title_screen(bool *flags, int &_COLOR_GRAY);

// Creates all windows for episode select, has all the episode select logic and destroys windows at the end
// returns 0 if episode has been selected
// returns 1 if terminal is smaller than 80x24
// returns -1 if option to go back has been pressed
int scene_episode_select(string &episode_path, int &inventory_size);

// Creates all windows for main game action, has all the main action logic and destroys windows at the end
// returns 1 if terminal is smaller than 80x24 or something else happened that should result in ending the program
// returns 2 if option to return to title screen has been selected
int scene_main_game_action(bool *flags, const string file_path, const int level_num, const int inventory_size, const int _COLOR_GRAY, int &score);