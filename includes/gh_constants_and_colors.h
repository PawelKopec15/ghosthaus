#pragma once

#include <iostream>
#include <ncurses.h>

#define TERM_MIN_WIDTH 80
#define TERM_MIN_HEIGHT 24

#define SQUARE_WIDTH 7
#define SQUARE_HEIGHT 3

#define RAW_MARGIN_SIDE 15
#define RAW_MARGIN_TOPBOTTOM 3
#define HORIZONTAL_BOARD_SHIFT -10
#define VERTICAL_BOARD_SHIFT -1

#define INVENTORY_WINDOW_WIDTH (RAW_MARGIN_SIDE + abs(HORIZONTAL_BOARD_SHIFT) - 5)

#define ENTITY_GRAPHICS_SIZE 12

#include "gh_class_item.h"

using std::string;

// 0 - no message
// 1 - you stare into the void
// 2 - i like that boulder
// 3 - the look of a tombstone
// 4 - splish splash
// 5 - red water
// 6 - no message (used for switchable floor/walls)
// 7 - table
extern const string tile_messages[8];

// 0 - void/floor
// 1 - path
// 2 - grass
// 3 - rock
// 4 - tombstone
// 5 - wall
// 6 - sign
// 7 - door closed
// 8 - door opened
// 9 - water
// 10 - table
// 11 - tree top
// 12 - tree bottom
// 13 - exit
// 14 - pillar
// 15 - switch
// 16 - switch floor
extern const char *tile_patterns[17];

// Spaces mean that no new character will be drawn on top of the tile
// Tabs mean that a single space character (entity's background color) will be drawn
// 0 - player
// 1 - key
// 2 - flashlight
// 3 - camera
// 4 - guard left
// 5 - guard right
// 6 - ghost left
// 7 - ghost right
// 8 - skeleton key
// 9 - gemstone
// 10 - stopwatch
// 11 - dead body
extern const string entity_graphics[ENTITY_GRAPHICS_SIZE];

// Function calls init_pair() a couple times so we have some colors for the UI
// ------------------------------------------------------------------------------------
// 1-9: ui and system stuff
// 1 - ui white on black
// 2 - ui danger
// 3 - ui debug
// 4 - ui white on magenta
// 5 - ui red on black
// 6 - ui black on blue
// 7 - ui cyan on blue
// 8 - ui black on white
void init_ui_pairs();

// Function calls init_pair() a couple times so we have some colors to work with
// requires specification as to what _COLOR_GRAY and _COLOR_FLOOR should be
// ------------------------------------------------------------------------------------
// 10-99: tiles
// 10 - void
// 11 - path / floor / switch1
// 12 - grass / tree / switch2 / switch floor
// 13 - rock / tombstone
// 14 - wall
// 15 - sign / table
// 16 - closed door
// 17 - opened door
// 18 - water
// 19 - red water
// 20 - green door
// 21 - sign
// 22 - exit
// 23 - red door
// 24 - switch wall
// 25 - switch 3 (red switch)
// ------------------------------------------------------------------------------------
// 100-199: entities
// 100 - player / ghost / skeleton key
// 101 - green key / green gemstone
// 102 - flashlight / camera / stopwatch
// 103 - red key / enemies / super flashlight / red gemstone / blood
// 104 - blue gemstone
void init_game_pairs(const int _COLOR_GRAY, const int _COLOR_FLOOR);

//* Items

extern const string _green_key_name;
extern const Item _green_key;

extern const string _red_key_name;
extern const Item _red_key;

extern const string _skeleton_key_name;
extern const Item _skeleton_key;

extern const string _flashlight_name;
extern const Item _flashlight;

extern const string _super_flashlight_name;
extern const Item _super_flashlight;

extern const string _camera_name;
extern const Item _camera;

extern const string _stopwatch_name;
extern const Item _stopwatch;

extern const string _blue_gem_name;
extern const Item _blue_gem;

extern const string _red_gem_name;
extern const Item _red_gem;

extern const string _green_gem_name;
extern const Item _green_gem;