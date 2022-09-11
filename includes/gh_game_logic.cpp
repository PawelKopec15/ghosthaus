#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <filesystem>

#include "gh_game_logic.h"

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
void find_flags(int argc, char const *argv[], bool *flags)
{
    flags[0] = false;
    flags[1] = false;

    for (int i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "-debug"))
            flags[0] = true;
        if (!strcmp(argv[i], "-nd") || !strcmp(argv[i], "-nodelay"))
            flags[1] = true;
    }
}

// Returns vector of .ep file paths in given folder
vector<string> find_ep_files(const string path)
{
    vector<string> toret;
    const string file_extention = ".ep";

    try
    {
        for (const auto &file : directory_iterator(path))
        {
            const string str = file.path().string();
            if (str.length() < file_extention.length())
                continue;
            if (str.compare(str.length() - file_extention.length(), file_extention.length(), file_extention) == 0)
                toret.push_back(str);
        }
        return toret;
    }
    catch (...)
    {
        vector<string> v;
        return v;
    }
}

// First line is line 1 !!!
string get_line_from_file(const string path, const unsigned int line)
{
    string toret;

    try
    {
        ifstream InputFile(path);

        for (unsigned int i = 0; i < line - 1; ++i)
        {
            InputFile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        getline(InputFile, toret);
        InputFile.close();
    }
    catch (...)
    {
        toret = "";
    }

    return toret;
}

// This function returns a two dimensional pointer of Tile objects and updates board_w and board_h.
// Also adds entity objects to their respective vectors, updates player and exit position.
// Also updates level_end_message and death_message
Tile **make_board(string file_path, const int level_num, int &board_w, int &board_h, const int squares_h, const int squares_v, Player &player, vector<Item> &ground_items, vector<Enemy *> &enemies, vector<TileManipulator *> &tileManipulators, vector<Sign> &signs, int &_COLOR_FLOOR, WINDOW *&error_win, int &exit_x, int &exit_y, string &level_end_message, string &death_message)
{
    string line;
    ifstream InputFile(file_path);

    wmove(error_win, 1, 0);

    if (!InputFile)
    {
        if (level_num == 1)
        {
            const string errormes = "Error opening file " + file_path + ".\n";
            wprintw(error_win, errormes.c_str());
        }
        return NULL;
    }

    int width = 0, height = 0;
    bool found_data = false;
    string level_end_mes = "Level complete!";
    string death_mes = "Level restart";

    string sign_messages[9] = {"", "", "", "", "", "", "", "", ""};

    int ghost_level = 1;
    string guard_kill_messages[4] = {"Wham! You are dead!", "Wham! You are done for!", "Wham! You didn't stand a chance!", "Wham! You are unalived!"};
    int guard_nums[4][3] = {{-1, 0, 0}, {0, -1, 0}, {-1, 0, 2}, {1, 0, 1}};

    int floor_color = COLOR_BLUE;

    while (!InputFile.eof())
    {
        getline(InputFile, line);
        line = delete_end_spaces(line);

        // it's really irritating there isn't a switch statement for strings in c++
        // width and height of the board
        if (line == "#width")
        {
            try
            {
                getline(InputFile, line);
                line = delete_end_spaces(line);
                width = stoi(line);
            }
            catch (...)
            {
                continue;
            }
        }
        else if (line == "#height")
        {
            try
            {
                getline(InputFile, line);
                line = delete_end_spaces(line);
                height = stoi(line);
            }
            catch (...)
            {
                continue;
            }
        }
        // signs
        else if (line.substr(0, 6) == "#sign ")
        {
            int signnum = 0;
            try
            {
                signnum = stoi(line.substr(6, 1));
            }
            catch (...)
            {
                continue;
            }

            try
            {
                getline(InputFile, line);
                line = delete_end_spaces(line);

                sign_messages[signnum - 1] = line;
            }
            catch (...)
            {
                continue;
            }
        }
        // ghost level
        else if (line == "#ghost" || line == "#ghost_level" || line == "#ghost_lvl")
        {
            try
            {
                getline(InputFile, line);
                line = delete_end_spaces(line);
                ghost_level = stoi(line);
            }
            catch (...)
            {
                continue;
            }
        }
        // guard kill message
        else if (line.substr(0, 19) == "#guard_killmessage ")
        {
            int guard_i = 0;
            try
            {
                guard_i = stoi(line.substr(19, 1));
            }
            catch (...)
            {
                continue;
            }

            try
            {
                getline(InputFile, line);
                line = delete_end_spaces(line);

                guard_kill_messages[guard_i - 1] = line;
            }
            catch (...)
            {
                continue;
            }
        }
        // guard direction
        else if (line.substr(0, 17) == "#guard_direction ")
        {
            int guard_i = 0;
            try
            {
                guard_i = stoi(line.substr(17, 1));
            }
            catch (...)
            {
                continue;
            }

            try
            {
                getline(InputFile, line);
                line = delete_end_spaces(line);

                guard_nums[guard_i - 1][0] = stoi(line);
            }
            catch (...)
            {
                continue;
            }
            try
            {
                getline(InputFile, line);
                line = delete_end_spaces(line);

                guard_nums[guard_i - 1][1] = stoi(line);
            }
            catch (...)
            {
                continue;
            }
        }
        // guard mode
        else if (line.substr(0, 12) == "#guard_mode ")
        {
            int guard_i = 0;
            try
            {
                guard_i = stoi(line.substr(12, 1));
            }
            catch (...)
            {
                continue;
            }

            try
            {
                getline(InputFile, line);
                line = delete_end_spaces(line);

                guard_nums[guard_i - 1][2] = stoi(line);
            }
            catch (...)
            {
                continue;
            }
        }
        // floor color
        else if (line == "#floor" || line == "#floor_color")
        {
            try
            {
                getline(InputFile, line);
                line = delete_end_spaces(line);

                if (line == "COLOR_BLUE" || line == "BLUE" || line == "blue" || line == "1")
                    floor_color = COLOR_BLUE;
                else if (line == "COLOR_BLACK" || line == "BLACK" || line == "black" || line == "0")
                    floor_color = COLOR_BLACK;
                else if (line == "COLOR_MAGENTA" || line == "MAGENTA" || line == "magenta" || line == "2")
                    floor_color = COLOR_MAGENTA;
                else
                    continue;
            }
            catch (...)
            {
                continue;
            }
        }
        // level end message
        else if (line == "#level_end_message" || line == "#end_message")
        {
            try
            {
                getline(InputFile, line);
                line = delete_end_spaces(line);
                level_end_mes = line;
            }
            catch (...)
            {
                continue;
            }
        }
        // level end message
        else if (line == "#death_message")
        {
            try
            {
                getline(InputFile, line);
                line = delete_end_spaces(line);
                death_mes = line;
            }
            catch (...)
            {
                continue;
            }
        }

        // skipping setup to go to tile data section
        else if (line == "#data" || line == "##data")
        {
            found_data = true;
            break;
        }
        // if nothing matches just skip, maybe it's an enter or some junk
        else
            continue;
    }

    // Error checks
    bool err = false;
    if (!found_data)
    {
        wprintw(error_win, "Board data ( #data ) not found.\n");
        err = true;
    }
    else if (width == 0)
    {
        wprintw(error_win, "Board width not found.\n");
        err = true;
    }
    else if (width < 0)
    {
        wprintw(error_win, "Invalid board width ( <0 ).\n");
        err = true;
    }
    if (height == 0)
    {
        wprintw(error_win, "Board height not found.\n");
        err = true;
    }
    else if (height < 0)
    {
        wprintw(error_win, "Invalid board height ( <0 ).\n");
        err = true;
    }
    if (err)
    {
        wprintw(error_win, "\nerror occurred while reading file:\n");
        wprintw(error_win, file_path.c_str());
        InputFile.close();
        return NULL;
    }

    // Most used tiles
    const Tile TILE_FLOOR = Tile(true, 0, 11);
    const Tile TILE_PATH = Tile(true, 1, 11);
    const Tile TILE_TABLE = Tile(false, 10, 15, 7);

    width = std::max(squares_h, width);
    height = std::max(squares_v, height);

    board_w = width;
    board_h = height;
    _COLOR_FLOOR = floor_color;
    level_end_message = level_end_mes;
    death_message = death_mes;

    Tile **currentboard = 0;
    currentboard = new Tile *[height];

    for (int i = 0; i < height; ++i)
    {
        try
        {
            getline(InputFile, line);
        }
        catch (...)
        {
            line = string(width, '0');
        }

        currentboard[i] = new Tile[width];

        for (int j = 0; j < width; ++j)
        {
            char c;
            try
            {
                c = line[j];
            }
            catch (...)
            {
                c = '0';
            }

            //* -----------
            //* MAIN SWITCH
            //* -----------

            switch (c)
            {

            //* Player / enemies
            case 'P': // player
            {
                player.set_position(j, i);
                currentboard[i][j] = TILE_FLOOR;
                break;
            }
            case 'G': // ghost
            {
                Enemy *en = new Ghost(j, i, entity_graphics[6], entity_graphics[7], 100, "\'\'there is no escape\'\'", ghost_level);
                enemies.push_back(en);
                currentboard[i][j] = TILE_FLOOR;
                break;
            }
            case 'a': // guard 1
            {
                Enemy *en = new GuardEnemy(j, i, entity_graphics[4], entity_graphics[5], 103, guard_kill_messages[0], guard_nums[0][0], guard_nums[0][1], guard_nums[0][2]);
                enemies.push_back(en);

                currentboard[i][j] = TILE_PATH;
                break;
            }
            case 'b': // guard 2
            {
                Enemy *en = new GuardEnemy(j, i, entity_graphics[5], entity_graphics[4], 103, guard_kill_messages[1], guard_nums[1][0], guard_nums[1][1], guard_nums[1][2]);
                enemies.push_back(en);

                currentboard[i][j] = TILE_PATH;
                break;
            }
            case 'c': // guard 3
            {
                Enemy *en = new GuardEnemy(j, i, entity_graphics[4], entity_graphics[5], 101, guard_kill_messages[2], guard_nums[2][0], guard_nums[2][1], guard_nums[2][2]);
                enemies.push_back(en);

                currentboard[i][j] = TILE_PATH;
                break;
            }
            case 'd': // guard 4
            {
                Enemy *en = new GuardEnemy(j, i, entity_graphics[5], entity_graphics[4], 101, guard_kill_messages[3], guard_nums[3][0], guard_nums[3][1], guard_nums[3][2]);
                enemies.push_back(en);

                currentboard[i][j] = TILE_PATH;
                break;
            }

            //* Passthrough
            case ' ': // floor
            {
                currentboard[i][j] = TILE_FLOOR;
                break;
            }
            case '.': // path
            {
                currentboard[i][j] = TILE_PATH;
                break;
            }
            case ',': // grass
            {
                currentboard[i][j] = Tile(true, 2, 12);
                break;
            }

            //* Not passthrough
            case '#': // brick wall
            {
                currentboard[i][j] = Tile(false, 5, 14);
                break;
            }
            case 'p': // pillar
            {
                currentboard[i][j] = Tile(false, 14, 13);
                break;
            }

            //* Not passthrough with message
            case 'r': // rock
            {
                currentboard[i][j] = Tile(false, 3, 13, 2);
                break;
            }
            case '+': // grave
            {
                currentboard[i][j] = Tile(false, 4, 13, 3);
                break;
            }
            case 'w': // water
            {
                currentboard[i][j] = Tile(false, 9, 18, 4);
                break;
            }
            case '~': // red water
            {
                currentboard[i][j] = Tile(false, 9, 19);
                break;
            }
            case 't': // table
            {
                currentboard[i][j] = TILE_TABLE;
                break;
            }

            //* Fake walls
            case '$': // fake brick wall
            {
                currentboard[i][j] = Tile(true, 5, 14);
                break;
            }
            case 'f': // fake rock
            {
                currentboard[i][j] = Tile(true, 3, 13);
                break;
            }

            //* Things that also modify tiles above them
            case '`': // floor and table above
            {
                if (i > 0)
                    currentboard[i - 1][j] = TILE_TABLE;
                currentboard[i][j] = TILE_FLOOR;
                break;
            }
            case 'T': // tree and upper tree above
            {
                if (i > 0)
                    currentboard[i - 1][j] = Tile(false, 11, 12);
                currentboard[i][j] = Tile(false, 12, 12);
                break;
            }

            //* Items
            case 'A': // skeleton (almighty) key
            {
                Item it = _skeleton_key;
                it.set_position(j, i);
                ground_items.push_back(it);

                currentboard[i][j] = TILE_FLOOR;
                break;
            }
            case 'K': // green key
            {
                Item it = _green_key;
                it.set_position(j, i);
                ground_items.push_back(it);

                currentboard[i][j] = TILE_FLOOR;
                break;
            }
            case 'L': // red key
            {
                Item it = _red_key;
                it.set_position(j, i);
                ground_items.push_back(it);

                currentboard[i][j] = TILE_FLOOR;
                break;
            }
            case 'F': // flashlight
            {
                Item it = _flashlight;
                it.set_position(j, i);
                ground_items.push_back(it);

                currentboard[i][j] = TILE_FLOOR;
                break;
            }
            case 'S': // stopwatch
            {
                Item it = _stopwatch;
                it.set_position(j, i);
                ground_items.push_back(it);

                currentboard[i][j] = TILE_FLOOR;
                break;
            }
            case 'C': // camera
            {
                Item it = _camera;
                it.set_position(j, i);
                ground_items.push_back(it);

                currentboard[i][j] = TILE_FLOOR;
                break;
            }
            case 'g': // blue gemstone
            {
                Item it = _blue_gem;
                it.set_position(j, i);
                ground_items.push_back(it);

                currentboard[i][j] = TILE_FLOOR;
                break;
            }
            case 'h': // red gemstone
            {
                Item it = _red_gem;
                it.set_position(j, i);
                ground_items.push_back(it);

                currentboard[i][j] = TILE_FLOOR;
                break;
            }
            case 'i': // green gemstone
            {
                Item it = _green_gem;
                it.set_position(j, i);
                ground_items.push_back(it);

                currentboard[i][j] = TILE_FLOOR;
                break;
            }

            //* Doors and exit
            case 'D': // unlocked door
            {
                currentboard[i][j] = Tile(false, 7, 16);

                TileManipulator *door = new Door(j, i, 0);
                tileManipulators.push_back(door);

                break;
            }
            case 'M': // green door (locked)
            {
                currentboard[i][j] = Tile(false, 7, 20);

                TileManipulator *door = new Door(j, i, 1);
                tileManipulators.push_back(door);
                break;
            }
            case 'N': // red door (locked)
            {
                currentboard[i][j] = Tile(false, 7, 23);

                TileManipulator *door = new Door(j, i, 2);
                tileManipulators.push_back(door);
                break;
            }
            case 'E': // exit
            {
                currentboard[i][j] = Tile(true, 13, 22);
                exit_x = j;
                exit_y = i;
                break;
            }

            //* Switch and co
            case '!': // switch
            {
                currentboard[i][j] = Tile(false, 15, 11);

                TileManipulator *s = new Switch(j, i);
                tileManipulators.push_back(s);

                break;
            }
            case '@': // switchable wall
            {
                currentboard[i][j] = Tile(false, 5, 24, 6);
                break;
            }
            case '_': // switchable floor
            {
                currentboard[i][j] = Tile(true, 16, 12, 6);
                break;
            }

            //* Signs
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            {
                const int num = c - '0';
                signs.push_back(Sign(j, i, sign_messages[num - 1]));

                currentboard[i][j] = Tile(false, 6, 21); // sign tile
                break;
            }

            //* Void
            case '0':
            default:
                currentboard[i][j] = Tile(); // defaults to void
                break;
            }
        }
    }

    InputFile.close();

    return currentboard;
}