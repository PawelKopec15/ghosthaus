#pragma once

#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <filesystem>

#include "gh_constants_and_colors.h"
#include "gh_class_tile.h"
#include "gh_class_player.h"
#include "gh_class_item.h"
#include "gh_classes_tilemanipulators.h"
#include "gh_classes_enemies.h"
#include "gh_class_sign.h"

using std::ifstream;
using std::string;
using std::vector;
using std::filesystem::directory_iterator;

// Checks if any flags have been inputted during program execution
// Updates bool values representing given flags
// Possible flags:
// flags[0] (debug):     -d or -debug
// flags[1] (no delay): -nd or -nodelay
void find_flags(int argc, char const *argv[], bool *flags);

// Returns vector of .ep file paths in given folder
vector<string> find_ep_files(const string path);

// First line is line 1 !!!
string get_line_from_file(const string path, const unsigned int line);

// This function returns a two dimensional pointer of Tile objects and updates board_w and board_h.
// Also adds entity objects to their respective vectors, updates player and exit position.
// Also updates level_end_message and death_message
Tile **make_board(string file_path, const int level_num, int &board_w, int &board_h, const int squares_h, const int squares_v, Player &player, vector<Item> &ground_items, vector<Enemy *> &enemies, vector<TileManipulator *> &tileManipulators, vector<Sign> &signs, int &_COLOR_FLOOR, WINDOW *&error_win, int &exit_x, int &exit_y, string &level_end_message, string &death_message);