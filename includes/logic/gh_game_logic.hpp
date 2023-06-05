#pragma once

#include <ncurses.h>

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../class/gh_class_item.hpp"
#include "../class/gh_class_player.hpp"
#include "../class/gh_class_sign.hpp"
#include "../class/gh_class_tile.hpp"
#include "../class/gh_classes_enemies.hpp"
#include "../class/gh_classes_tilemanipulators.hpp"
#include "../singleton/gh_singleton_board.hpp"
#include "../singleton/gh_singleton_debugger.hpp"
#include "gh_constants_and_colors.hpp"

using std::ifstream;
using std::string;
using std::vector;
using std::filesystem::directory_iterator;

// Checks if any flags have been inputted during program execution
// Updates bool values representing given flags
void find_flags(int argc, char const *argv[], bool &debug, bool &nodelay);

// Returns vector of .ep file paths in given folder
vector<string> find_ep_files(const string &path);

// First line is line 1 !!!
string get_line_from_file(const string &path, const unsigned int line);

// This function returns a two dimensional pointer of Tile objects and updates board_w and board_h.
// Also adds entity objects to their respective vectors, updates player and exit position.
// Also updates level_end_message and death_message
void make_board(const string &file_path, const int level_num, Player &player);