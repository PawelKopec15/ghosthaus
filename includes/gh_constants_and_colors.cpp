#include <iostream>
#include <ncurses.h>

#include "gh_constants_and_colors.h"

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
const string tile_messages[8] =
    {
        "",                                            // no message
        "You stare into the void...",                  // void
        "I like that boulder, that is a nice boulder", // rock
        "Bodies fill the fields I see,",               // tombstone
        "~ splish ~ splash ~ c++ ~ is ~ trash ~",      // water
        "Better not to think what this red water is",  // red water
        "",                                            // no message (for switch)
        "It's a table. There is nothing on it."        // table
};

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
const char *tile_patterns[17] =
    {
        "                     ", // void/floor
        ".   .     .    .    .", // path
        ",,,,,, ,, ,,,,, ,,, ,", // grass
        " ▄▓▄   ▐█▓▓█▄ █▓█▓▓█▌", // rock
        "   ▄    ▀▀█▀▀ ▐▓███▓▌", // tombstone
        "■■░■■░■■░■■░■■■■░■■░■", // wall
        "████████ !!! ██▄▄ ▄▄█", // sign
        "█·   ·██·   o██·   ·█", // door closed
        "█     ██     ██     █", // door opened
        "~~~~~~~~~~~~~~~~~~~~~", // water
        "       ▓█▓▓▓█▓█▀▀▀▀▀█", // table
        "   ▄     █▓█   ▐█▓█▌ ", // tree top
        "▐█▓▓█▓▌█▓██▓▓█   █   ", // tree bottom
        "█·   ·██EXIT!██·   ·█", // exit
        "▀▓███▓▀▓██▓██▓▄▓█▓█▓▄", // pillar
        "   _    -(!)-  ▄▓█▓▄ ", // switch
        "  _  _  _  _    _  _ ", // switch floor

};

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
const string entity_graphics[ENTITY_GRAPHICS_SIZE] =
    {
        "   ()    /[]\\    ||  ",
        "         O--w",
        "        |)==X",
        "  ___,  |_O_|",
        "A (O)  |/[_]\\ | | |",
        "  (O) A /[_]\\|  | | |",
        " (o\to)  |\tO\t\\   `~~~'",
        " (o\to)  /\tO\t| '~~~'",
        "  (')     |_     |=",
        "   /\\     \\/",
        " ._O_.  /\t|\t\\  \\___/",
        "         .#,.   *'#'",
};

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
void init_ui_pairs()
{
    init_pair(1, COLOR_WHITE, COLOR_BLACK);   // ui white on black
    init_pair(2, COLOR_WHITE, COLOR_RED);     // ui danger
    init_pair(3, COLOR_BLACK, COLOR_CYAN);    // ui debug
    init_pair(4, COLOR_WHITE, COLOR_MAGENTA); // ui white on magenta
    init_pair(5, COLOR_RED, COLOR_BLACK);     // ui red on black
    init_pair(6, COLOR_BLACK, COLOR_BLUE);    // ui black on blue
    init_pair(7, COLOR_CYAN, COLOR_BLUE);     // ui cyan on blue
    init_pair(8, COLOR_BLACK, COLOR_WHITE);   // ui black on white
}

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
void init_game_pairs(const int _COLOR_GRAY, const int _COLOR_FLOOR)
{
    init_pair(10, COLOR_MAGENTA, COLOR_BLACK); // void
    init_pair(11, COLOR_WHITE, _COLOR_FLOOR);  // path / floor / switch1
    init_pair(12, COLOR_GREEN, _COLOR_FLOOR);  // grass / tree / switch2 / switch floor
    init_pair(13, _COLOR_GRAY, _COLOR_FLOOR);  // rock/tombstone
    init_pair(14, COLOR_RED, COLOR_WHITE);     // wall
    init_pair(15, COLOR_YELLOW, _COLOR_FLOOR); // table
    init_pair(16, COLOR_WHITE, COLOR_YELLOW);  // closed door
    init_pair(17, COLOR_WHITE, _COLOR_FLOOR);  // opened door
    init_pair(18, COLOR_WHITE, COLOR_CYAN);    // water
    init_pair(19, COLOR_YELLOW, COLOR_RED);    // red water
    init_pair(20, COLOR_WHITE, COLOR_GREEN);   // green door
    init_pair(21, _COLOR_FLOOR, COLOR_YELLOW); // sign
    init_pair(22, COLOR_GREEN, COLOR_YELLOW);  // exit
    init_pair(23, COLOR_WHITE, COLOR_RED);     // red door
    init_pair(24, COLOR_GREEN, COLOR_WHITE);   // switch wall
    init_pair(25, COLOR_RED, _COLOR_FLOOR);    // switch 3

    init_pair(100, COLOR_WHITE, _COLOR_FLOOR);  // player / ghost / skeleton key
    init_pair(101, COLOR_GREEN, _COLOR_FLOOR);  // green key / green gemstone
    init_pair(102, COLOR_YELLOW, _COLOR_FLOOR); // flashlight / camera / stopwatch
    init_pair(103, COLOR_RED, _COLOR_FLOOR);    // red key / enemies / super flashlight / red gemstone / blood
    init_pair(104, COLOR_CYAN, _COLOR_FLOOR);   // blue gemstone
}

//* Items

const string _green_key_name = "green key";
const Item _green_key(0, 0, entity_graphics[1], 101, _green_key_name, "You found a green key! You can use it to unlock a green door.", 0);

const string _red_key_name = "red key";
const Item _red_key(0, 0, entity_graphics[1], 103, _red_key_name, "You found a red key! You can use it to unlock a red door.", 0);

const string _skeleton_key_name = "skeleton key";
const Item _skeleton_key(0, 0, entity_graphics[8], 100, _skeleton_key_name, "You found a skeleton key! It can open any door and is indestructible!", 0);

const string _flashlight_name = "flashlight";
const Item _flashlight(0, 0, entity_graphics[2], 102, _flashlight_name, "You found a flashlight! Your field of view increases.", 0);

const string _super_flashlight_name = "super light";
const Item _super_flashlight(0, 0, entity_graphics[2], 103, _super_flashlight_name, "You found a super flashlight! How did you do that?", 0);

const string _camera_name = "camera";
const Item _camera(0, 0, entity_graphics[3], 102, _camera_name, "You found a camera! Press C to use it's flash.", 15, 0);

const string _stopwatch_name = "stopwatch";
const Item _stopwatch(0, 0, entity_graphics[10], 102, _stopwatch_name, "You found a stopwatch! Press S to freeze time temporarily.", 25, 0);

const string _blue_gem_name = "blue gem";
const Item _blue_gem(0, 0, entity_graphics[9], 104, _blue_gem_name, "You found a blue gemstone! It's worth 1000 points.", 1000);

const string _red_gem_name = "red gem";
const Item _red_gem(0, 0, entity_graphics[9], 103, _red_gem_name, "You found a red gemstone! It's worth 500 points.", 500);

const string _green_gem_name = "green gem";
const Item _green_gem(0, 0, entity_graphics[9], 101, _green_gem_name, "You found a green gemstone! It's worth 200 points.", 200);