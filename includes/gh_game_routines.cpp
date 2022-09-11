#include <ncurses.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <chrono>

#include "gh_game_routines.h"

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

#define t_x player.get_target_x()
#define t_y player.get_target_y()

using std::vector;

// Checks to see if testmes should replace currentmes
message more_important_message(message currentmes, message testmes)
{
    if (testmes.priority <= currentmes.priority && testmes.text != "")
        return testmes;
    return currentmes;
}

// Should be the first thing to happen in the program to set up ncurses
// returns 0 if everything is rather alright or 1 if it was unable to set up locale.
int setup_ncurses_routine()
{
    // Setting up locale
    if (setlocale(LC_ALL, "") == NULL)
    {
        std::cout << std::endl
                  << std::endl
                  << "Unable to setlocale: game will not display properly." << std::endl;
        sleep(5);
        return (1);
    }

    // Setting up ncurses
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    keypad(stdscr, TRUE);

    if (!has_colors())
    {
        printw("It seems that your terminal does not support color.\nA terminal with color is recommended.");
        getch();
    }
    else
    {
        start_color();
    }

    init_ui_pairs();

    return (0);
}

// If terminal is no less than 80x24 returns 0
// Else prints message, waits for getch() and returns 1
int verify_terminal_size_routine(int &terminal_w, int &terminal_h)
{
    if (get_terminal_dimentions(terminal_w, terminal_h) == 1)
    {
        printw("This game requires terminal to be at least 80 columns by 24 rows.");
        getch();
        endwin();
        return (1);
    }
    return (0);
}

// Title screen loop
int title_screen_loop_routine(int &selected_option, const int page, WINDOW *&win_options, int &_COLOR_GRAY)
{
    // drawing win_options
    werase(win_options);
    box(win_options, '+', '+');

    switch (page)
    {
    case 0:
        mvwprintw(win_options, 2, 18, "NEW GAME");
        mvwprintw(win_options, 4, 18, "CONTINUE (WIP)");
        mvwprintw(win_options, 6, 18, "SETTINGS (INDEV)");
        mvwprintw(win_options, 8, 20, "QUIT");
        if (selected_option == 5)
            mvwprintw(win_options, 10, 18, "?? ?? ??");
        break;
    case 1:
        wattron(win_options, COLOR_PAIR(5));
        mvwprintw(win_options, 5, 6, "https://youtu.be/K56NRnOzPso?t=15");
        wattroff(win_options, COLOR_PAIR(5));
        break;
    case 2:

        // gray color settings

        mvwprintw(win_options, 2, 8, "Gray color: ");

        wattron(win_options, A_BOLD);

        wattron(win_options, COLOR_PAIR(6));
        if (_COLOR_GRAY == COLOR_BLACK)
            wattron(win_options, A_UNDERLINE);
        wprintw(win_options, "this █    ");
        wattroff(win_options, A_UNDERLINE);

        wattron(win_options, COLOR_PAIR(7));
        if (_COLOR_GRAY == COLOR_CYAN)
            wattron(win_options, A_UNDERLINE);
        wprintw(win_options, "this █");
        wattroff(win_options, A_UNDERLINE);

        wattroff(win_options, A_BOLD);
        wattron(win_options, COLOR_PAIR(1));
        mvwprintw(win_options, 2, 26, " or ");

        // printing back
        mvwprintw(win_options, getmaxy(win_options) - 2, 18, "<-- BACK");

    default:
        break;
    }

    wattron(win_options, COLOR_PAIR(5));

    mvwprintw(win_options, 2 * selected_option, 5, "->");
    mvwprintw(win_options, 2 * selected_option, getmaxx(win_options) - 7, "<-");

    wattroff(win_options, COLOR_PAIR(5));

    wrefresh(win_options);

    // getting input
    flushinp();
    int ch = getch();
    if (simple_option_cursing(ch, selected_option))
        return selected_option;

    restrict_option(selected_option, 1, 5);

    return (0);
}

// Episode select loop
int episode_select_loop_routine(int &selected_option, int &page, const int page_count, const int page_height, WINDOW *&win_episode_select, vector<string> episode_file_paths)
{

    werase(win_episode_select);
    box(win_episode_select, 'x', 0);

    // printing first lines from file paths

    const int a = episode_file_paths.size() - page * page_height;
    const int episode_names_to_print = std::min(page_height, a);

    for (int i = 0; i < episode_names_to_print; ++i)
    {
        string str = get_line_from_file(episode_file_paths.at(i + page * page_height), 1);
        mvwprintw(win_episode_select, i + 2, (getmaxx(win_episode_select) - str.length()) / 2, str.c_str());
    }

    // printing back
    mvwprintw(win_episode_select, getmaxy(win_episode_select) - 2, 18, "<-- BACK");

    // printing page
    if (page_count > 1)
        mvwprintw(win_episode_select, 0, 18, (" page " + std::to_string(page) + " ").c_str());

    // printing pgup pgdn
    if (page_count - 1 > page)
        mvwprintw(win_episode_select, getmaxy(win_episode_select) - 1, getmaxx(win_episode_select) - 12, "PgDn->");
    if (page > 0)
        mvwprintw(win_episode_select, getmaxy(win_episode_select) - 1, 6, "<-PgUp");

    // printing arrows
    int i_have_no_energy_to_think_up_interesting_var_names_anymore;
    selected_option == page_height + 1 ? i_have_no_energy_to_think_up_interesting_var_names_anymore = 1 : i_have_no_energy_to_think_up_interesting_var_names_anymore = 0;

    wattron(win_episode_select, COLOR_PAIR(5));

    mvwprintw(win_episode_select, selected_option + 1 + i_have_no_energy_to_think_up_interesting_var_names_anymore, 3, "->");
    mvwprintw(win_episode_select, selected_option + 1 + i_have_no_energy_to_think_up_interesting_var_names_anymore, getmaxx(win_episode_select) - 5, "<-");

    wattroff(win_episode_select, COLOR_PAIR(5));

    wrefresh(win_episode_select);

    // input
    int ch = getch();
    switch (ch)
    {
    case KEY_NPAGE:
        page++;
        break;
    case KEY_PPAGE:
        page--;
        break;
    default:
        if (simple_option_cursing(ch, selected_option))
            return selected_option;
        break;
    }

    restrict(page, 0, page_count - 1);
    restrict_option(selected_option, 1, page_height + 1);

    return (0);
}

// Erases screen, then prints all lines from episode_tile_path (beginning at line 4)
void new_game_messageprint_subroutine(string episode_file_path, const int terminal_w, const int terminal_h)
{
    erase();
    refresh();
    for (int i = 4;; ++i)
    {
        const string str = get_line_from_file(episode_file_path, i);
        if (str == "")
            break;
        mvprintw(terminal_h / 4 + i, (terminal_w - str.length()) / 2, str.c_str());
        refresh();
        getch();
    }
}

// Erases screen, prints level_end_message and contains animation and functionality for adding up score
void level_end_subroutine(vector<Item> items, int &score, const string level_end_message, const int terminal_w, const int terminal_h, Camera camera)
{
    erase();
    refresh();

    WINDOW *win_item_display = newwin(SQUARE_HEIGHT, SQUARE_WIDTH, terminal_h / 2 - 3, (terminal_w + 4) / 2);

    mvprintw(terminal_h / 2 - 8, (terminal_w - level_end_message.length()) / 2, level_end_message.c_str());
    mvprintw(terminal_h / 2 - 6, (terminal_w - 14) / 2, "score: ");

    for (auto it = items.begin(); it != items.end(); ++it)
    {
        mvprintw(terminal_h / 2 - 6, (terminal_w - 14) / 2 + 7, (std::to_string(score) + "   ").c_str());
        refresh();
        wrefresh(win_item_display);
        usleep(700000);

        const int score_inc = it->get_score();

        mvprintw(terminal_h / 2 - 2, (terminal_w - 10) / 2, ("+ " + std::to_string(score_inc) + "   ").c_str());
        VisibleEntity *ve = &*it;
        werase(win_item_display);
        wrefresh(win_item_display);
        usleep(100000);
        camera.draw_entity(*ve, win_item_display);
        wrefresh(win_item_display);

        score += score_inc;
    }

    mvprintw(terminal_h / 2 - 6, (terminal_w - 14) / 2 + 7, (std::to_string(score) + "   ").c_str());
    refresh();
    sleep(2);

    delwin(win_item_display);
    sleep(1);
}

// Erases the screen, decreases score by 100, prints death_message
void level_restart_subroutine(int &score, const string death_message, const int terminal_w, const int terminal_h)
{
    erase();

    mvprintw(terminal_h / 2 - 8, (terminal_w - death_message.length()) / 2, death_message.c_str());
    mvprintw(terminal_h / 2 - 6, (terminal_w - 14) / 2, ("score: " + std::to_string(score) + " -100").c_str());
    refresh();
    sleep(2);

    score -= 100;
    mvprintw(terminal_h / 2 - 6, (terminal_w - 14) / 2, ("score: " + std::to_string(score) + "        ").c_str());
    refresh();
    sleep(2);
}

// Erases the screen and prints episode finish message, waits for getch() to end
void episode_end_subroutine(const int score, const int terminal_w, const int terminal_h)
{
    erase();

    WINDOW *win = newwin(7, 50, terminal_h / 2 - 10, (terminal_w - 50) / 2);
    box(win, 0, 0);

    mvwprintw(win, 2, 9, "-- You finished this episode! --");
    mvwprintw(win, 4, 12, "final score: ");
    wprintw(win, std::to_string(score).c_str());

    mvprintw(terminal_h / 2 + 2, (terminal_w - 40) / 2, "press any key to go back to title screen");

    refresh();
    wrefresh(win);

    flushinp();
    getch();

    delwin(win);
}

// Tries to add item to inventory and checks for special cases
// Returns 0 if successful or 1 if inventory is full
// May update currentmes
int item_gain_subroutine(Item item, Inventory &inventory, message &currentmes, int &flashlight_lvl)
{
    message testmes = item.targetmessage();

    if (inventory.add_item(item) == 0)
    {

        // checking for special items
        // flashlight
        if (item.get_name() == _flashlight_name)
        {
            if (flashlight_lvl > 0)
                testmes.text = "You already have a flashlight";
            else
                flashlight_lvl = 1;
        }
        else if (item.get_name() == _super_flashlight_name)
            flashlight_lvl = 4;

        currentmes = more_important_message(currentmes, testmes);
        return (0);
    }
    else
    {
        // if could not collect item, replace message text with a warning
        testmes.text = "Inventory full, didn't collect item";
        currentmes = more_important_message(currentmes, testmes);
        return (1);
    }
}

// Drops item to the ground (a.k.a. adds said item to ground_items )
// Checks for special cases (flashlight, super flashlight)
// Updates testmes
void item_lose_subroutine(Inventory &inventory, vector<Item> &ground_items, message &testmes, const int player_x, const int player_y, WINDOW *&win_inventory, const int inventory_page, const int item_selected_to_drop, Camera &camera, int &flashlight_lvl, vector<vector<WINDOW *>> &win_squares, const int squares_h, const int squares_v)
{
    Item dropped_item = inventory.drop_item(player_x, player_y, win_inventory, inventory_page, item_selected_to_drop);
    ground_items.push_back(dropped_item);
    string dropped_item_name = dropped_item.get_name();

    testmes.text = "Dropped " + dropped_item_name + " on the ground";

    // checking for special items
    // flashlight
    if (dropped_item_name == _flashlight_name && !inventory.has(_flashlight_name) && flashlight_lvl == 1)
    {
        testmes.text = "You dropped your " + _flashlight_name + ", your field of view decreases";
        flashlight_lvl = 0;
    }
    // super flashlight
    if (dropped_item_name == _super_flashlight_name && !inventory.has(_super_flashlight_name))
    {
        testmes.text = "You dropped your super flashlight, what a shame";
        if (inventory.has(_flashlight_name))
            flashlight_lvl = 1;
        else
            flashlight_lvl = 0;
    }
}

// Takes vector of TileManipulator pointers and runs turnhandler on each
// May update currentmes
void handle_tilemanip_turnhandlers_subroutine(message &currentmes, vector<TileManipulator *> &tileManipulators, Tile **board)
{
    message testmes;
    for (size_t i = 0; i < tileManipulators.size(); ++i)
    {
        testmes = tileManipulators[i]->turnhandler(board);
        currentmes = more_important_message(currentmes, testmes);
    }
}

// Renders pretty much everything, that includes win_squares, win_inventory and win_messages !! but not win_debug !!
void main_action_render_subroutine(Player &player, vector<Enemy *> &enemies, Inventory &inventory, int &inventory_page, vector<Item> &ground_items, Camera &camera, const int squares_h, const int squares_v, vector<vector<WINDOW *>> &win_squares, WINDOW *&win_messages, WINDOW *&win_inventory, Tile **board, const int board_w, const int board_h, message currentmes, const ActionMode action_mode, int &item_selected_to_drop, const int flashlight_lvl)
{
    camera.set_position(player.get_x(), player.get_y());

    // Rendering win_squares
    vector<VisibleEntity *> visibleEntities;

    //? visibleEntities: Items, Enemies, Player
    for (size_t i = 0; i < ground_items.size(); ++i)
        visibleEntities.push_back(&ground_items[i]);

    if (action_mode == am_dead) // if player is dead, he should be below the enemies
        visibleEntities.push_back(&player);

    for (size_t i = 0; i < enemies.size(); ++i)
        visibleEntities.push_back(*&enemies[i]);

    if (action_mode != am_dead) // if player is not dead, he should be displayed on top of enemies
        visibleEntities.push_back(&player);

    camera.draw_flashlight_circle(flashlight_lvl, squares_h, squares_v, win_squares, board, board_w, board_h, visibleEntities);
    // Drawing inventory
    inventory.draw_inventory(win_inventory, inventory_page, action_mode == am_inventory_drop, item_selected_to_drop);

    // Printing to win_messages
    camera.update_win_messages(win_messages, currentmes.text);
}

// Makes win_squares shine bright, renders all win_squares (if show_full_screen = true), then makes the screen go black so flashlight circle can be rendered
void screen_flash_subroutine(const bool show_full_screen, Player &player, vector<Enemy *> &enemies, vector<Item> &ground_items, Camera &camera, const int squares_h, const int squares_v, vector<vector<WINDOW *>> &win_squares, Tile **board, const int board_w, const int board_h)
{
    vector<VisibleEntity *> visibleEntities;
    //? visibleEntities: Items, Enemies, Player
    for (size_t i = 0; i < ground_items.size(); ++i)
        visibleEntities.push_back(&ground_items[i]);

    for (size_t i = 0; i < enemies.size(); ++i)
        visibleEntities.push_back(*&enemies[i]);

    visibleEntities.push_back(&player);

    camera.effect_flash(show_full_screen, squares_h, squares_v, win_squares, board, board_w, board_h, visibleEntities);
}

// Sets player params and action mode to dead related, runs through 10 iterations of Enemy and TileManipulator turnhandlers
void player_kill_subroutine(Player &player, const message mes, vector<Enemy *> &enemies, vector<TileManipulator *> &tileManipulators, Inventory &inventory, int &inventory_page, vector<Item> &ground_items, Camera &camera, const int squares_h, const int squares_v, vector<vector<WINDOW *>> &win_squares, WINDOW *&win_messages, WINDOW *&win_inventory, Tile **board, const int board_w, const int board_h, ActionMode &action_mode, int &item_selected_to_drop, const int flashlight_lvl)
{
    player.set_dead(true);
    player.set_graphic(entity_graphics[11]);
    player.set_color_pair(103);
    action_mode = am_dead;

    main_action_render_subroutine(player, enemies, inventory, inventory_page, ground_items, camera, squares_h, squares_v, win_squares, win_messages, win_inventory, board, board_w, board_h, mes, action_mode, item_selected_to_drop, flashlight_lvl);

    sleep(1);
    message m;
    for (int i = 0; i < 10; ++i)
    {
        // moving enemies
        for (size_t j = 0; j < enemies.size();)
        {
            m = enemies[j]->turnhandler(board, board_w, board_h, player, std::min(flashlight_lvl + 2, (int)squares_h / 2), std::min(flashlight_lvl + 2, (int)squares_v / 2), enemies);

            if (m.text == "#deletemeplease" && m.priority == 11)
            {
                enemies.erase(enemies.begin() + j);
            }
            else
                ++j;
        }
        // moving tile manipulators
        handle_tilemanip_turnhandlers_subroutine(m, tileManipulators, board);

        main_action_render_subroutine(player, enemies, inventory, inventory_page, ground_items, camera, squares_h, squares_v, win_squares, win_messages, win_inventory, board, board_w, board_h, mes, action_mode, item_selected_to_drop, flashlight_lvl);

        usleep(400000);
    }
}

// Main game loop
// Returns: 0 if the loop should run again
//          1 if game should quit
//          2 if the game should go back to title screen
//      and 3 if player has been killed so all variables should be set to default and stuff
int main_action_loop_routine(Player &player, Inventory &inventory, int &inventory_page, int &tilemanip_turns_freeze, int &enemy_turns_freeze, vector<TileManipulator *> &tileManipulators, vector<Enemy *> &enemies, vector<Sign> &signs, vector<Item> &ground_items, Camera &camera, const int squares_h, const int squares_v, vector<vector<WINDOW *>> &win_squares, WINDOW *&win_messages, WINDOW *&win_inventory, WINDOW *&win_debug, Tile **board, const int board_w, const int board_h, const int exit_x, const int exit_y, bool *flags, ActionMode &action_mode, int &item_selected_to_drop, int &flashlight_lvl, bool &invincible)
{
    if (!flags[1] && action_mode != am_command)
        usleep(60000);
    message currentmes;
    currentmes.priority = 10;
    message testmes;
    int ch;
    bool arrow_pressed = true;

    // handling turns_freezed
    if (action_mode == am_normal)
    {
        tilemanip_turns_freeze = std::max(0, tilemanip_turns_freeze - 1);
        enemy_turns_freeze = std::max(0, enemy_turns_freeze - 1);
    }

    //* Player position, arrow input section

    if (action_mode != am_command)
        flushinp();
    ch = getch();

    switch (action_mode)
    {
    case am_inventory_drop:
        // If in this mode don't interact with anything, just move cursor
        switch (ch)
        {
        case KEY_UP:
        case KEY_RIGHT:
            item_selected_to_drop--;
            break;
        case KEY_DOWN:
        case KEY_LEFT:
            item_selected_to_drop++;
            break;
        default:
            arrow_pressed = false;
        }
        break;

    case am_command:
    {
        string str = camera.get_last_message_text();
        if (ch == 10) // key enter - executing command
        {

            while (str.size() > 0) // deleting spaces at the end
            {
                if (str.at(str.size() - 1) == ' ')
                    str = str.substr(0, str.size() - 1);
                else
                    break;
            }

            if (str == ":q!" || str == ":wq" || str == ":exit" || str == ":quit")
                return (1);
            else if (str.substr(0, 6) == ":give ")
            {
                const string subject = str.substr(6, str.size() - 1);
                if (subject == _green_key_name)
                    item_gain_subroutine(_green_key, inventory, currentmes, flashlight_lvl);

                else if (subject == _flashlight_name)
                    item_gain_subroutine(_flashlight, inventory, currentmes, flashlight_lvl);

                else if (subject == _camera_name)
                    item_gain_subroutine(_camera, inventory, currentmes, flashlight_lvl);

                else if (subject == _stopwatch_name)
                    item_gain_subroutine(_stopwatch, inventory, currentmes, flashlight_lvl);

                else if (subject == _super_flashlight_name || subject == "super flashlight")
                    item_gain_subroutine(_super_flashlight, inventory, currentmes, flashlight_lvl);

                else if (subject == _skeleton_key_name)
                    item_gain_subroutine(_skeleton_key, inventory, currentmes, flashlight_lvl);

                else if (subject == _red_key_name)
                    item_gain_subroutine(_red_key, inventory, currentmes, flashlight_lvl);

                else if (subject == _blue_gem_name)
                    item_gain_subroutine(_blue_gem, inventory, currentmes, flashlight_lvl);

                else if (subject == _red_gem_name)
                    item_gain_subroutine(_red_gem, inventory, currentmes, flashlight_lvl);

                else if (subject == _green_gem_name)
                    item_gain_subroutine(_green_gem, inventory, currentmes, flashlight_lvl);

                else
                    currentmes.text = ":spooky error 16 - no item \"" + subject + "\"";
            }
            else if (str == ":light up")
                flashlight_lvl++;

            else if (str == ":light down")
                flashlight_lvl = std::max(flashlight_lvl - 1, 0);

            else if (str == ":invincible")
            {
                invincible = !invincible;
                currentmes.text = "Invincibility set to ";
                if (invincible)
                    currentmes.text += "true";
                else
                    currentmes.text += "false";
            }

            else if (str == ":die")
            {
                message m;
                m.text = "Programmer wins. Fatality.";
                curs_set(0);
                player_kill_subroutine(player, m, enemies, tileManipulators, inventory, inventory_page, ground_items, camera, squares_h, squares_v, win_squares, win_messages, win_inventory, board, board_w, board_h, action_mode, item_selected_to_drop, flashlight_lvl);
                return (3);
            }

            // if no valid command was inputted, and command line is not empty
            else if (str != "" && str != ":")
                currentmes.text = ":spooky error 15 - invalid command";

            action_mode = am_normal;
            curs_set(0);
        }
        else if (ch == KEY_BACKSPACE) // erasing last character
        {
            currentmes.text = str.substr(0, str.size() - 1);
        }
        else // inputting a character
        {
            testmes.text = ch;
            currentmes.text = str + testmes.text;
        }
        break;
    }

    case am_normal:
    default:
        curs_set(0);
        player.set_target_position(player.get_x(), player.get_y());
        // Handling arrow input -> moving target position
        switch (ch)
        {
        case KEY_UP:
            player.move_target(0, -1, board_w, board_h);
            break;
        case KEY_DOWN:
            player.move_target(0, 1, board_w, board_h);
            break;
        case KEY_LEFT:
            player.move_target(-1, 0, board_w, board_h);
            break;
        case KEY_RIGHT:
            player.move_target(1, 0, board_w, board_h);
            break;
        case ' ':
            // skipping turn (debug only)
            if (flags[0])
                break;
        default:
            arrow_pressed = false;
            tilemanip_turns_freeze++;
            enemy_turns_freeze++;
        }

        // Checking if we can pass through the tiles
        if (arrow_pressed && board[t_y][t_x].is_passable())
        {
            player.set_position(t_x, t_y);
        }

        // Getting message from tile, setting as new default
        testmes.text = tile_messages[board[t_y][t_x].get_message_num()];
        testmes.priority = 7;
        currentmes = more_important_message(currentmes, testmes);
        break;
    }

    //* Other input, inventory handling section
    // If arrow wasn't pressed, check for other inputs
    // exit function if no valid button has been pressed
    if (!arrow_pressed)
    {
        switch (ch)
        {

        case KEY_NPAGE:
            inventory_page++;
            break;
        case KEY_PPAGE:
            inventory_page--;
            break;

        case 'd':
        case 'D':
            if (action_mode != am_inventory_drop && !inventory.is_empty())
            {
                action_mode = am_inventory_drop;

                testmes.text = "Select item do drop, confirm with Enter or return with D";
                testmes.priority = 7;
                currentmes = more_important_message(currentmes, testmes);

                item_selected_to_drop = 0;
            }
            else if (inventory.is_empty())
            {
                testmes.text = "You have no items, and nothing to drop";
                testmes.priority = 7;
                currentmes = more_important_message(currentmes, testmes);
            }
            else
            {
                action_mode = am_normal;
            }
            break;

        // Items
        // camera
        case 'c':
        case 'C':
            if (action_mode == am_normal)
            {
                testmes.priority = 7;
                if (inventory.use_item(_camera_name, false) == 0)
                {
                    enemy_turns_freeze += 3;
                    camera.update_win_messages(win_messages, "Say cheese!");
                    screen_flash_subroutine(true, player, enemies, ground_items, camera, squares_h, squares_v, win_squares, board, board_w, board_h);
                }
                else if (inventory.has(_camera_name))
                    testmes.text = "Your camera isn't ready.";
                else
                    testmes.text = "You don't have a camera.";

                currentmes = more_important_message(currentmes, testmes);
                break;
            }
        // camera
        case 's':
        case 'S':
            if (action_mode == am_normal)
            {
                testmes.priority = 7;
                if (inventory.use_item(_stopwatch_name, false) == 0)
                {
                    enemy_turns_freeze += 2;
                    tilemanip_turns_freeze += 2;
                    camera.update_win_messages(win_messages, "Suddenly, everything around you freezes...");
                    screen_flash_subroutine(false, player, enemies, ground_items, camera, squares_h, squares_v, win_squares, board, board_w, board_h);
                }
                else if (inventory.has(_camera_name))
                    testmes.text = "Your stopwatch isn't ready.";
                else
                    testmes.text = "You don't have a stopwatch.";

                currentmes = more_important_message(currentmes, testmes);
                break;
            }

        case 10: // enter key
            if (action_mode == am_inventory_drop)
            {
                item_lose_subroutine(inventory, ground_items, testmes, player.get_x(), player.get_y(), win_inventory, inventory_page, item_selected_to_drop, camera, flashlight_lvl, win_squares, squares_h, squares_v);

                testmes.priority = 7;
                currentmes = more_important_message(currentmes, testmes);

                action_mode = am_normal;
                enemy_turns_freeze--;
                tilemanip_turns_freeze--;
            }
            else
                return (0);
            break;

        case (KEY_F0 + 7):
            if (camera.get_last_message_text() == "Press F7 again to exit back to title screen")
                return (2);
            camera.update_win_messages(win_messages, "Press F7 again to exit back to title screen");
            return (0);

        case (KEY_F0 + 10):
            if (camera.get_last_message_text() == "Press F10 again to quit game without saving")
                return (1);
            camera.update_win_messages(win_messages, "Press F10 again to quit game without saving");
            return (0);

        // command mode
        case ':':
            if (flags[0] && action_mode == am_normal)
            {
                currentmes.priority = 1;
                currentmes.text = ":";
                action_mode = am_command;
                curs_set(2);
            }
            else
                return (0);
            break;

        default:
            return (0);
        }
    }

    // have to leave it as is to avoid errors at the end of the file
    //? Debug: start time measure
    auto debug_time_begin = std::chrono::high_resolution_clock::now();

    //* TileManipulator, Sign and Item targethandler section
    if (arrow_pressed && action_mode == am_normal)
    {
        // TileManipulators
        for (size_t i = 0; i < tileManipulators.size(); ++i)
        {
            if (tileManipulators[i]->get_x() == t_x && tileManipulators[i]->get_y() == t_y)
            {
                testmes = tileManipulators[i]->targethandler(board, board_h, board_w, inventory);
                currentmes = more_important_message(currentmes, testmes);
            }
        }

        // Signs
        for (auto it = begin(signs); it != end(signs); ++it)
        {
            if (it->get_x() == t_x && it->get_y() == t_y)
            {
                testmes = it->targetmessage();
                currentmes = more_important_message(currentmes, testmes);
            }
        }

        // Items
        for (auto it = begin(ground_items); it != end(ground_items);)
        {
            if (it->get_x() == t_x && it->get_y() == t_y)
            {
                if (!item_gain_subroutine(*it, inventory, currentmes, flashlight_lvl))
                    it = ground_items.erase(it);
                else
                    // if inventory is full then checking other items is pointless, just break
                    break;
            }
            else
            {
                ++it;
            }
        }
    }

    //* Enemy handling section
    if (enemy_turns_freeze == 0 && action_mode == am_normal)
    {
        ////handle_enemy_turnhandlers_subroutine(currentmes, player, enemies, board, board_w, board_h, squares_h, squares_v, flashlight_lvl);
        message testmes;

        for (size_t i = 0; i < enemies.size();)
        {

            // Checking for if the enemy should kill us before moving
            if (enemies[i]->get_x() == player.get_x() && enemies[i]->get_y() == player.get_y() && !invincible)
            {
                player_kill_subroutine(player, enemies[i]->targetmessage(), enemies, tileManipulators, inventory, inventory_page, ground_items, camera, squares_h, squares_v, win_squares, win_messages, win_inventory, board, board_w, board_h, action_mode, item_selected_to_drop, flashlight_lvl);
                return (3);
            }

            testmes = enemies[i]->turnhandler(board, board_w, board_h, player, std::min(flashlight_lvl + 2, (int)squares_h / 2), std::min(flashlight_lvl + 2, (int)squares_v / 2), enemies);

            if (testmes.text == "#deletemeplease" && testmes.priority == 11)
            {
                enemies.erase(enemies.begin() + i);
            }
            else
            {
                // checking if enemy collided with player after moving
                if (enemies[i]->get_x() == player.get_x() && enemies[i]->get_y() == player.get_y() && !invincible)
                {
                    player_kill_subroutine(player, enemies[i]->targetmessage(), enemies, tileManipulators, inventory, inventory_page, ground_items, camera, squares_h, squares_v, win_squares, win_messages, win_inventory, board, board_w, board_h, action_mode, item_selected_to_drop, flashlight_lvl);
                    return (3);
                }

                currentmes = more_important_message(currentmes, testmes);

                ++i;
            }
        }
    }

    //* TileManipulator turnhandler section
    if (tilemanip_turns_freeze == 0 && action_mode == am_normal)
        handle_tilemanip_turnhandlers_subroutine(currentmes, tileManipulators, board);

    //* Increment power of items
    if (arrow_pressed && action_mode == am_normal)
        inventory.increment_power_of_items();

    //* Render section

    if (flags[0]) //? Debug: end time measure
    {
        auto debug_time_end = std::chrono::high_resolution_clock::now();
        camera.update_win_debug(win_debug, std::chrono::duration_cast<std::chrono::nanoseconds>(debug_time_end - debug_time_begin).count(), ch, action_mode, tilemanip_turns_freeze, enemy_turns_freeze, flashlight_lvl, invincible);
    }

    main_action_render_subroutine(player, enemies, inventory, inventory_page, ground_items, camera, squares_h, squares_v, win_squares, win_messages, win_inventory, board, board_w, board_h, currentmes, action_mode, item_selected_to_drop, flashlight_lvl);

    //* Checking if we've reached the exit
    if (t_x == exit_x && t_y == exit_y)
    {
        usleep(200000);
        return (-1);
    }

    return (0);
}
