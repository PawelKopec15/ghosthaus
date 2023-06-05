#pragma once

#include <ncurses.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <string>

#include "logic/gh_game_logic.hpp"
#include "logic/gh_game_routines.hpp"
#include "logic/gh_game_scenes.hpp"
#include "singleton/gh_singleton_debugger.hpp"
#include "singleton/gh_singleton_inventory.hpp"

using std::string;

int ghosthaus(int argc, char const *argv[]);