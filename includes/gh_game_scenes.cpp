#include <ncurses.h>
#include <iostream>
#include <vector>

#include "gh_game_scenes.h"

#include "gh_simple_functions.h"
#include "gh_game_routines.h"
#include "gh_game_logic.h"

using std::vector;

// Creates all windows for title screen, has all the title screen logic and destroys windows at the end
// returns 0 if option to start a new game has been selected
// returns 1 if terminal is smaller than 80x24 or game should end
int scene_title_screen(bool *flags, int &_COLOR_GRAY)
{
    curs_set(1);
    curs_set(0);
    int terminal_w, terminal_h;
    if (verify_terminal_size_routine(terminal_w, terminal_h))
        return (1);

    int selected_option = 1;
    int page = 0;
    WINDOW *win_logo;
    WINDOW *win_options;

    erase();
    mvprintw(terminal_h - 2, terminal_w - 10, "ver 1.0");
    refresh();

    setup_title_screen_windows(win_logo, win_options, terminal_w, terminal_h);

    if (flags[0])
        mvprintw(terminal_h - 2, 0, "Debug enabled");
    if (flags[1])
        mvprintw(terminal_h - 1, 0, "No delay enabled");

    while (1)
    {
        if (title_screen_loop_routine(selected_option, page, win_options, _COLOR_GRAY))
        {
            switch (page)
            {

            case 0:
                switch (selected_option)
                {
                case 1:
                    delwin(win_logo);
                    delwin(win_options);
                    return (0);
                case 3:
                    page = 2;
                    break;
                case 4:
                    delwin(win_logo);
                    delwin(win_options);
                    endwin();
                    return (1);
                case 5:
                    page = 1;
                    break;
                }
                break;

            case 2:
                switch (selected_option)
                {
                case 1:
                    if (_COLOR_GRAY == COLOR_BLACK)
                        _COLOR_GRAY = COLOR_CYAN;
                    else
                        _COLOR_GRAY = COLOR_BLACK;
                    break;
                default:
                    page = 0;
                    break;
                }
                break;

            default:
                page = 0;
                break;
            }
        }
    }
}

// Creates all windows for episode select, has all the episode select logic and destroys windows at the end
// returns 0 if episode has been selected
// returns 1 if terminal is smaller than 80x24
// returns -1 if option to go back has been pressed
int scene_episode_select(string &episode_path, int &inventory_size)
{
    curs_set(0);
    int terminal_w, terminal_h;
    if (verify_terminal_size_routine(terminal_w, terminal_h))
        return (1);

    WINDOW *win_episode_select;
    setup_episode_select_windows(win_episode_select, terminal_w, terminal_h);

    attron(COLOR_PAIR(5));
    mvprintw((terminal_h - 22) / 2 + 9, (terminal_w - 44) / 2, "            -- SELECT EPISODE --            ");
    attroff(COLOR_PAIR(5));

    vector<string> episode_file_paths = find_ep_files("episodes");
    if (episode_file_paths.size() == 0)
    {
        mvwprintw(win_episode_select, 3, 1, "No .ep files found in /episodes.\n");
        flushinp();
        getch();

        delwin(win_episode_select);
        return (-1);
    }

    const unsigned int page_height = getmaxy(win_episode_select) - 5;
    const int page_count = get_page_count(episode_file_paths.size(), page_height);

    int selected_option = 1;
    int page = 0;

    while (1)
    {
        unsigned int main_return_val = episode_select_loop_routine(selected_option, page, page_count, page_height, win_episode_select, episode_file_paths);
        if (main_return_val == page_height + 1)
        {
            delwin(win_episode_select);
            return (-1);
        }
        else if (main_return_val > 0 && main_return_val <= episode_file_paths.size())
        {
            episode_path = get_line_from_file(episode_file_paths[selected_option - 1 + page * page_height], 2);
            inventory_size = stoi(get_line_from_file(episode_file_paths[selected_option - 1 + page * page_height], 3));

            new_game_messageprint_subroutine(episode_file_paths[selected_option - 1 + page * page_height], terminal_w, terminal_h);

            return (0);
        }
    }
}

// Creates all windows for main game action, has all the main action logic and destroys windows at the end
// returns 1 if terminal is smaller than 80x24 or something else happened that should result in ending the program
// returns 2 if option to return to title screen has been selected
int scene_main_game_action(bool *flags, const string file_path, const int level_num, const int inventory_size, const int _COLOR_GRAY, int &score)
{
    int terminal_w, terminal_h;
    if (verify_terminal_size_routine(terminal_w, terminal_h))
        return (1);

    int squares_h, squares_v;
    int margin_side, margin_topbottom;

    WINDOW *win_messages;
    WINDOW *win_inventory;
    WINDOW *win_main_border;
    WINDOW *win_debug;
    vector<vector<WINDOW *>> win_squares;

    setup_action_windows(win_messages, win_inventory, win_main_border, win_debug, win_squares, terminal_w, terminal_h, squares_h, squares_v, margin_side, margin_topbottom);

respawn_point:

    erase();
    refresh();

    Player player(0, 0, entity_graphics[0], 100);

    Inventory inventory(inventory_size);

    vector<TileManipulator *> tileManipulators;
    vector<Enemy *> enemies;
    vector<Sign> signs;
    vector<Item> ground_items;
    Camera camera(0, 0);

    int inventory_page = 0;
    int tilemanip_turns_freeze = 0;
    int enemy_turns_freeze = 0;

    ActionMode action_mode = am_normal;
    int item_selected_to_drop = 0;

    int flashlight_lvl = 0;

    bool invincible = false;

    string level_end_message;
    string death_message;
    int board_w, board_h;
    int _COLOR_FLOOR;
    int exit_x = 0, exit_y = 0;

    Tile **board = make_board(file_path, level_num, board_w, board_h, squares_h, squares_v, player, ground_items, enemies, tileManipulators, signs, _COLOR_FLOOR, win_main_border, exit_x, exit_y, level_end_message, death_message);
    if (board == NULL) // if error occurred
    {
        if (level_num == 1)
        {
            wrefresh(win_main_border);
            flushinp();
            getch();
            endwin();
        }
        delwin(win_messages);
        delwin(win_inventory);
        delwin(win_main_border);
        delwin(win_debug);
        return (1);
    }

    box(win_main_border, 0, 0);
    wrefresh(win_main_border);
    camera.draw_full_screen_darkness(squares_h, squares_v, win_squares);

    init_game_pairs(_COLOR_GRAY, _COLOR_FLOOR);

    main_action_render_subroutine(player, enemies, inventory, inventory_page, ground_items, camera, squares_h, squares_v, win_squares, win_messages, win_inventory, board, board_w, board_h, message(), action_mode, item_selected_to_drop, flashlight_lvl);

    while (1)
    {
        const int main_return_val = main_action_loop_routine(player, inventory, inventory_page, tilemanip_turns_freeze, enemy_turns_freeze, tileManipulators, enemies, signs, ground_items, camera, squares_h, squares_v, win_squares, win_messages, win_inventory, win_debug, board, board_w, board_h, exit_x, exit_y, flags, action_mode, item_selected_to_drop, flashlight_lvl, invincible);
        if (main_return_val == 1)
        {
            delwin(win_messages);
            delwin(win_inventory);
            delwin(win_main_border);
            delwin(win_debug);
            endwin();
            return (1);
        }
        else if (main_return_val == 2)
        {
            delwin(win_messages);
            delwin(win_inventory);
            delwin(win_main_border);
            delwin(win_debug);
            return (2);
        }
        else if (main_return_val == 3)
        {
            level_restart_subroutine(score, death_message, terminal_w, terminal_h);
            goto respawn_point;
        }
        else if (main_return_val == -1)
            break;
    }

    delwin(win_messages);
    delwin(win_inventory);
    delwin(win_main_border);
    delwin(win_debug);

    //* Level end moment

    init_game_pairs(_COLOR_GRAY, COLOR_BLACK);
    level_end_subroutine(inventory.get_all_items(), score, level_end_message, terminal_w, terminal_h, camera);

    erase();
    refresh();
    usleep(500000);

    return (0);
}