#include <ncurses.h>
#include <vector>
#include <unistd.h>

#include "gh_class_camera.h"

#include "gh_class_tile.h"
#include "gh_class_visibleentity.h"
#include "gh_constants_and_colors.h"
#include "gh_simple_functions.h"
#include "gh_enum_action_mode.h"

using std::vector;

//* Camera -> extends PositionObject
// Constains render related functions

Camera::Camera(int x, int y) : PositionObject(x, y)
{
    this->last_flashlight_lvl = 0;
}

string Camera::get_last_message_text()
{
    return (last_message_text);
}

// Function updates window responsible for showing messages
void Camera::update_win_messages(WINDOW *&win_messages, const string text)
{
    werase(win_messages);
    box(win_messages, 0, 0);
    mvwprintw(win_messages, 1, 1, text.c_str());
    last_message_text = text;
    wrefresh(win_messages);
}

// Function updates the window that displays debug information
void Camera::update_win_debug(WINDOW *&win_debug, const int elapsed_time, const int last_char, ActionMode action_mode, const int tilemanip_turns_freeze, const int enemy_turns_freeze, const int flashlight_lvl, const bool invincible)
{
    werase(win_debug);

    wattron(win_debug, A_REVERSE);
    wprintw(win_debug, "-DEBUG-");
    mvwprintw(win_debug, 0, 15, std::to_string(elapsed_time).c_str());
    mvwprintw(win_debug, 0, 28, std::to_string(last_char).c_str());
    string str;
    switch (action_mode)
    {
    case am_normal:
        str = "normal";
        break;
    case am_inventory_drop:
        str = "inv_drop";
        break;
    case am_command:
        str = "command";
        break;
    case am_dead:
        str = "dead";
        break;
    }
    mvwprintw(win_debug, 0, 37, str.c_str());
    mvwprintw(win_debug, 0, 52, std::to_string(tilemanip_turns_freeze).c_str());
    mvwprintw(win_debug, 0, 54, std::to_string(enemy_turns_freeze).c_str());
    mvwprintw(win_debug, 0, 63, std::to_string(flashlight_lvl).c_str());
    if (invincible)
        str = "tru";
    else
        str = "fal";
    mvwprintw(win_debug, 0, 71, str.c_str());

    wattroff(win_debug, A_REVERSE);
    mvwprintw(win_debug, 0, 10, "time:");
    mvwprintw(win_debug, 0, 24, "in:");
    mvwprintw(win_debug, 0, 33, "am:");
    mvwprintw(win_debug, 0, 46, "t_fr:");
    mvwprintw(win_debug, 0, 57, "flvl:");
    mvwprintw(win_debug, 0, 66, "inv:");

    wrefresh(win_debug);
}

// Function draws a single entity on a given single square
void Camera::draw_entity(VisibleEntity &visible_entity, WINDOW *&square)
{
    // entity drawing, witch ignores white space
    wattron(square, A_BOLD);
    wattron(square, COLOR_PAIR(visible_entity.get_color_pair()));
    string entity_graphic = visible_entity.get_graphic();

    for (unsigned int c = 0; c < entity_graphic.length(); c++) // haha c++
    {
        char cc = entity_graphic[c];
        if (cc == '\t') // \t will be interpreted as actually drawing a space char
            mvwprintw(square, c / SQUARE_WIDTH, c - SQUARE_WIDTH * (c / SQUARE_WIDTH), " ");
        else if (cc != ' ') // actual space will be ignored (used to iterate c)
        {
            const char ccc[2] = {cc, '\0'}; // necessary, as mvwprintw() function prints garbage if it can't find \0
            mvwprintw(square, c / SQUARE_WIDTH, c - SQUARE_WIDTH * (c / SQUARE_WIDTH), ccc);
        }
    }
}

// Function turns all squares white, then renders all squares (if show_full_screen), then turns all squares black
void Camera::effect_flash(const bool show_full_screen, const int squares_h, const int squares_v, vector<vector<WINDOW *>> &win_squares, Tile **board, int board_w, int board_h, vector<VisibleEntity *> &visible_entities)
{
    // white flash
    draw_full_screen_flash(squares_h, squares_v, win_squares);
    usleep(100000);
    if (show_full_screen)
    {
        // drawing full screen
        draw_full_screen(squares_h, squares_v, win_squares, board, board_w, board_h, visible_entities);
        usleep(1500000);
        // black screen
        draw_full_screen_darkness(squares_h, squares_v, win_squares);
    }
    else
        usleep(2000000);
    usleep(100000);
    // bugfix
    draw_darkness_around(last_flashlight_lvl, squares_h, squares_v, win_squares);
}

// Function renders screen (or rather updates windows in win_squares).
// It changes camera's x and y values if they're too close to the border, so that it doesn't try to get invalid table indexes.
void Camera::draw_full_screen(const int squares_h, const int squares_v, vector<vector<WINDOW *>> &win_squares, Tile **board, const int board_w, const int board_h, vector<VisibleEntity *> &visible_entities)
{
    restrict(x, squares_h / 2, board_w - squares_h / 2 - 1);
    restrict(y, squares_v / 2, board_h - squares_v / 2 - 1);
    y = std::min(board_h - squares_v / 2 - 1, y);

    for (int i = 0; i < squares_v; ++i)
    {
        for (int j = 0; j < squares_h; ++j)
        {

            int current_y = y - squares_v / 2 + i;
            int current_x = x - squares_h / 2 + j;

            //* Drawing tiles from board
            Tile currentTile = board[current_y][current_x];
            draw_square(current_x, current_y, visible_entities, currentTile, win_squares[i][j]);
        }
    }
}

// Function turns all win_squares black
void Camera::draw_full_screen_darkness(int squares_h, int squares_v, vector<vector<WINDOW *>> &win_squares)
{
    for (int i = 0; i < squares_v; ++i)
    {
        for (int j = 0; j < squares_h; ++j)
        {
            draw_black_square(win_squares[i][j]);
        }
    }
}

// Function turns all win_squares white
void Camera::draw_full_screen_flash(int squares_h, int squares_v, vector<vector<WINDOW *>> &win_squares)
{
    for (int i = 0; i < squares_v; ++i)
    {
        for (int j = 0; j < squares_h; ++j)
        {
            draw_colored_square(win_squares[i][j], 8);
        }
    }
}

// Function renders screen (updates win_squares) around the center bounded by a semi circle thingy (dependant on flashlight_lvl)
// If flashlight level is lower than before, it also draws darkness around
void Camera::draw_flashlight_circle(int flashlight_lvl, const int squares_h, const int squares_v, vector<vector<WINDOW *>> &win_squares, Tile **board, const int board_w, const int board_h, vector<VisibleEntity *> &visible_entities)
{
    restrict_camera_pos(squares_h, squares_v, board_w, board_h);

    int val_1 = ((squares_v - 5) / 2) - flashlight_lvl;
    int val_2 = ((squares_h - 5) / 2) - flashlight_lvl;

    //* Checking if flashlight level has been lowered
    if (last_flashlight_lvl > flashlight_lvl)
        draw_darkness_around(flashlight_lvl, squares_h, squares_v, win_squares);
    last_flashlight_lvl = flashlight_lvl;

    //* Drawing tiles
    // top and bottom rows
    if (val_1 >= 0)
    {
        for (int j = val_2 + 1; j < (squares_h - val_2 - 1); ++j)
        {
            int current_y1 = y - squares_v / 2 + val_1;
            int current_y2 = y - squares_v / 2 + squares_v - val_1 - 1;
            int current_x = x - squares_h / 2 + j;

            // better not to look at this code for too long
            draw_square(current_x, current_y1, visible_entities, board[current_y1][current_x], win_squares[val_1][j]);
            draw_square(current_x, current_y2, visible_entities, board[current_y2][current_x], win_squares[squares_v - val_1 - 1][j]);
        }
    }

    // middle part
    const int ibound = std::min(squares_v - val_1 - 1, squares_v);
    const int jbound = std::min(squares_h - val_2, squares_h);
    for (int i = std::max(val_1 + 1, 0); i < ibound; ++i)
    {
        for (int j = std::max(val_2, 0); j < jbound; ++j)
        {

            int current_y = y - squares_v / 2 + i;
            int current_x = x - squares_h / 2 + j;

            //* Drawing tiles from board
            Tile currentTile = board[current_y][current_x];
            draw_square(current_x, current_y, visible_entities, currentTile, win_squares[i][j]);
        }
    }
}
