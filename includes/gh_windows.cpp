#include <iostream>
#include <ncurses.h>
#include <locale.h>
#include <vector>
#include <math.h>

#include "gh_windows.h"

#include "gh_constants_and_colors.h"

using std::vector;

// Saves terminal dimentions into given variables
// If terminal is smaller than TERM_MIN_WIDTH x TERM_MIN_HEIGHT it returns 1
int get_terminal_dimentions(int &terminal_w, int &terminal_h)
{
    getmaxyx(stdscr, terminal_h, terminal_w);

    if (terminal_h < TERM_MIN_HEIGHT || terminal_w < TERM_MIN_WIDTH)
    {
        return (1);
    }

    return (0);
}

// Sets up windows for episode select
int setup_episode_select_windows(WINDOW *&win_adventure_select, const int terminal_w, const int terminal_h)
{
    refresh();

    win_adventure_select = newwin(12, 44, (terminal_h - 22) / 2 + 10, (terminal_w - 44) / 2);
    box(win_adventure_select, 'x', 0);
    wrefresh(win_adventure_select);

    return 0;
}

// Sets up windows for title screen
int setup_title_screen_windows(WINDOW *&win_logo, WINDOW *&win_options, const int terminal_w, const int terminal_h)
{

    win_logo = newwin(9, 79, 1, (terminal_w - 79) / 2);
    wbkgd(win_logo, COLOR_PAIR(1));
    wprintw(win_logo, "  ▄████  ██░ ██  ▒█████    ██████ ▄▄▄█████▓ ██░ ██  ▄▄▄       █    ██   ██████ ");
    wprintw(win_logo, " ██▒ ▀█▒▓██░ ██▒▒██▒  ██▒▒██    ▒ ▓  ██▒ ▓▒▓██░ ██▒▒████▄     ██  ▓██▒▒██    ▒ ");
    wprintw(win_logo, "▒██░▄▄▄░▒██▀▀██░▒██░  ██▒░ ▓██▄   ▒ ▓██░ ▒░▒██▀▀██░▒██  ▀█▄  ▓██  ▒██░░ ▓██▄   ");
    wprintw(win_logo, "░▓█  ██▓░▓█ ░██ ▒██   ██░  ▒   ██▒░ ▓██▓ ░ ░▓█ ░██ ░██▄▄▄▄██ ▓▓█  ░██░  ▒   ██▒");
    wprintw(win_logo, "░▒▓███▀▒░▓█▒░██▓░ ████▓▒░▒██████▒▒  ▒██▒ ░ ░▓█▒░██▓ ▓█   ▓██▒▒▒█████▓ ▒██████▒▒");
    wprintw(win_logo, " ░▒   ▒  ▒ ░░▒░▒░ ▒░▒░▒░ ▒ ▒▓▒ ▒ ░  ▒ ░░    ▒ ░░▒░▒ ▒▒   ▓▒█░░▒▓▒ ▒ ▒ ▒ ▒▓▒ ▒ ░");
    wattron(win_logo, COLOR_PAIR(5));
    wprintw(win_logo, "  ░   ░  ▒ ░▒░ ░  ░ ▒ ▒░ ░ ░▒  ░ ░    ░     ▒ ░▒░ ░  ▒   ▒▒ ░░░▒░ ░ ░ ░ ░▒  ░ ░");
    wprintw(win_logo, "░ ░   ░  ░  ░░ ░░ ░ ░ ▒  ░  ░  ░    ░       ░  ░░ ░  ░   ▒    ░░░ ░ ░ ░  ░  ░  ");
    wprintw(win_logo, "      ░  ░  ░  ░    ░ ░        ░            ░  ░  ░      ░  ░   ░           ░  ");
    wrefresh(win_logo);

    win_options = newwin(12, 44, (terminal_h - 22) / 2 + 10, (terminal_w - 44) / 2);
    box(win_options, '+', '+');
    wrefresh(win_options);

    return (0);
}

// Sets up all nessecary WINDOW* variables for the action of the game
// makes use of many #define directives
int setup_action_windows(WINDOW *&win_messages, WINDOW *&win_inventory, WINDOW *&win_main_border, WINDOW *&win_debug, vector<vector<WINDOW *>> &win_squares, const int terminal_w, const int terminal_h, int &squares_h, int &squares_v, int &margin_side, int &margin_topbottom)
{
    erase();
    bkgd(COLOR_PAIR(1));
    refresh();

    // setting up number of squares and margin sides
    squares_h = floor((terminal_w - 2 * RAW_MARGIN_SIDE) / SQUARE_WIDTH);
    if (squares_h % 2 == 0)
        squares_h--;
    squares_v = floor((terminal_h - 2 * RAW_MARGIN_TOPBOTTOM) / SQUARE_HEIGHT);
    if (squares_v % 2 == 0)
        squares_v--;

    margin_side = floor((terminal_w - squares_h * SQUARE_WIDTH) / 2);
    margin_topbottom = floor((terminal_h - squares_v * SQUARE_HEIGHT) / 2);

    // window to be the border of the main window
    win_main_border = newwin(squares_v * SQUARE_HEIGHT + 2, squares_h * SQUARE_WIDTH + 4, margin_topbottom - 1 + VERTICAL_BOARD_SHIFT, margin_side - 2 + HORIZONTAL_BOARD_SHIFT);

    // window to display messages (like when you collect an item or bump into a rock)
    win_messages = newwin(3, terminal_w - 2 * (margin_side - 2 + HORIZONTAL_BOARD_SHIFT), terminal_h - 3, margin_side - 2 + HORIZONTAL_BOARD_SHIFT);
    box(win_messages, 0, 0);

    // window to display information about collected items
    win_inventory = newwin(squares_v * SQUARE_HEIGHT + 2, INVENTORY_WINDOW_WIDTH, margin_topbottom - 1 + VERTICAL_BOARD_SHIFT, terminal_w - margin_side + HORIZONTAL_BOARD_SHIFT + 2);

    // 2d vector table of windows, each representing a single square to render
    vector<WINDOW *> win_squares_row(squares_h, 0);
    vector<vector<WINDOW *>> temp_win_squares(squares_v, win_squares_row);
    for (int i = 0; i < squares_v; ++i)
    {
        for (int j = 0; j < squares_h; ++j)
        {
            temp_win_squares[i][j] = newwin(SQUARE_HEIGHT, SQUARE_WIDTH, margin_topbottom + i * SQUARE_HEIGHT + VERTICAL_BOARD_SHIFT, margin_side + j * SQUARE_WIDTH + HORIZONTAL_BOARD_SHIFT);
        }
    }
    win_squares = temp_win_squares;

    // window to debug stuff in (single horizontal line)
    win_debug = newwin(1, terminal_w, 0, 0);
    wbkgd(win_debug, COLOR_PAIR(3));

    return (0);
}
