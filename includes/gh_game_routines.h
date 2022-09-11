#pragma once

#include <ncurses.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <chrono>

#include "gh_windows.h"
#include "gh_class_player.h"
#include "gh_classes_tilemanipulators.h"
#include "gh_class_sign.h"
#include "gh_class_inventory.h"
#include "gh_class_camera.h"
#include "gh_constants_and_colors.h"
#include "gh_simple_functions.h"
#include "gh_enum_action_mode.h"
#include "gh_game_logic.h"

using std::vector;

// Checks to see if testmes should replace currentmes
message more_important_message(message currentmes, message testmes);

// Should be the first thing to happen in the program to set up ncurses
// returns 0 if everything is rather alright or 1 if it was unable to set up locale.
int setup_ncurses_routine();

// If terminal is no less than 80x24 returns 0
// Else prints message, waits for getch() and returns 1
int verify_terminal_size_routine(int &terminal_w, int &terminal_h);

// Title screen loop
int title_screen_loop_routine(int &selected_option, const int page, WINDOW *&win_options, int &_COLOR_GRAY);

// Episode select loop
int episode_select_loop_routine(int &selected_option, int &page, const int page_count, const int page_height, WINDOW *&win_episode_select, vector<string> episode_file_paths);

// Erases screen, then prints all lines from episode_tile_path (beginning at line 4)
void new_game_messageprint_subroutine(string episode_file_path, const int terminal_w, const int terminal_h);

// Erases screen, prints level_end_message and contains animation and functionality for adding up score
void level_end_subroutine(vector<Item> items, int &score, const string level_end_message, const int terminal_w, const int terminal_h, Camera camera);

// Erases the screen, decreases score by 100, prints death_message
void level_restart_subroutine(int &score, const string death_message, const int terminal_w, const int terminal_h);

// Erases the screen and prints episode finish message, waits for getch() to end
void episode_end_subroutine(const int score, const int terminal_w, const int terminal_h);

// Tries to add item to inventory and checks for special cases
// Returns 0 if successful or 1 if inventory is full
// May update currentmes
int item_gain_subroutine(Item item, Inventory &inventory, message &currentmes, int &flashlight_lvl);

// Drops item to the ground (a.k.a. adds said item to ground_items )
// Checks for special cases (flashlight, super flashlight)
// Updates testmes
void item_lose_subroutine(Inventory &inventory, vector<Item> &ground_items, message &testmes, const int player_x, const int player_y, WINDOW *&win_inventory, const int inventory_page, const int item_selected_to_drop, Camera &camera, int &flashlight_lvl, vector<vector<WINDOW *>> &win_squares, const int squares_h, const int squares_v);

// Takes vector of TileManipulator pointers and runs turnhandler on each
// May update currentmes
void handle_tilemanip_turnhandlers_subroutine(message &currentmes, vector<TileManipulator *> &tileManipulators, Tile **board);

// Renders pretty much everything, that includes win_squares, win_inventory and win_messages !! but not win_debug !!
void main_action_render_subroutine(Player &player, vector<Enemy *> &enemies, Inventory &inventory, int &inventory_page, vector<Item> &ground_items, Camera &camera, const int squares_h, const int squares_v, vector<vector<WINDOW *>> &win_squares, WINDOW *&win_messages, WINDOW *&win_inventory, Tile **board, const int board_w, const int board_h, message currentmes, const ActionMode action_mode, int &item_selected_to_drop, const int flashlight_lvl);

// Makes win_squares shine bright, renders all win_squares (if show_full_screen = true), then makes the screen go black so flashlight circle can be rendered
void screen_flash_subroutine(const bool show_full_screen, Player &player, vector<Enemy *> &enemies, vector<Item> &ground_items, Camera &camera, const int squares_h, const int squares_v, vector<vector<WINDOW *>> &win_squares, Tile **board, const int board_w, const int board_h);

// Sets player params and action mode to dead related, runs through 10 iterations of Enemy and TileManipulator turnhandlers
void player_kill_subroutine(Player &player, const message mes, vector<Enemy *> &enemies, vector<TileManipulator *> &tileManipulators, Inventory &inventory, int &inventory_page, vector<Item> &ground_items, Camera &camera, const int squares_h, const int squares_v, vector<vector<WINDOW *>> &win_squares, WINDOW *&win_messages, WINDOW *&win_inventory, Tile **board, const int board_w, const int board_h, ActionMode &action_mode, int &item_selected_to_drop, const int flashlight_lvl);

// Main game loop
// Returns: 0 if the loop should run again
//          1 if game should quit
//          2 if the game should go back to title screen
//      and 3 if player has been killed so all variables should be set to default and stuff
int main_action_loop_routine(Player &player, Inventory &inventory, int &inventory_page, int &tilemanip_turns_freeze, int &enemy_turns_freeze, vector<TileManipulator *> &tileManipulators, vector<Enemy *> &enemies, vector<Sign> &signs, vector<Item> &ground_items, Camera &camera, const int squares_h, const int squares_v, vector<vector<WINDOW *>> &win_squares, WINDOW *&win_messages, WINDOW *&win_inventory, WINDOW *&win_debug, Tile **board, const int board_w, const int board_h, const int exit_x, const int exit_y, bool *flags, ActionMode &action_mode, int &item_selected_to_drop, int &flashlight_lvl, bool &invincible);