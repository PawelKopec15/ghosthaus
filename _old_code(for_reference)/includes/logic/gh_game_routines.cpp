#include "gh_game_routines.hpp"

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
	if (setlocale(LC_ALL, "") == nullptr)
	{
		std::cout << std::endl << std::endl << "Unable to setlocale: game will not display properly." << std::endl;
		sleep(5);
		return 1;
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

	return 0;
}

// If terminal is no less than 80x24 returns 0
// Else prints message, waits for getch() and returns 1
int verify_terminal_size_routine(int &terminal_w, int &terminal_h)
{
	if (get_terminal_dimensions(terminal_w, terminal_h) == 1)
	{
		printw("This game requires terminal to be at least 80 columns by 24 rows.");
		getch();
		endwin();
		return 1;
	}
	return 0;
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

	restrict_wrap_around(selected_option, 1, 5);

	return 0;
}

// Episode select loop
int episode_select_loop_routine(int &selected_option, int &page, const int page_count, const int page_height,
								WINDOW *&win_episode_select, vector<string> episode_file_paths)
{
	werase(win_episode_select);
	box(win_episode_select, 'x', 0);

	// printing first lines from file paths

	const int a                      = episode_file_paths.size() - page * page_height;
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
	selected_option == page_height + 1 ? i_have_no_energy_to_think_up_interesting_var_names_anymore = 1
									   : i_have_no_energy_to_think_up_interesting_var_names_anymore = 0;

	wattron(win_episode_select, COLOR_PAIR(5));

	mvwprintw(win_episode_select, selected_option + 1 + i_have_no_energy_to_think_up_interesting_var_names_anymore, 3,
			  "->");
	mvwprintw(win_episode_select, selected_option + 1 + i_have_no_energy_to_think_up_interesting_var_names_anymore,
			  getmaxx(win_episode_select) - 5, "<-");

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
	restrict_wrap_around(selected_option, 1, page_height + 1);

	return 0;
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
void level_end_subroutine(vector<Item> items, int &score, const int terminal_w, const int terminal_h)
{
	Debugger::Get().Log("Finished level.");

	erase();
	refresh();

	WINDOW *win_item_display = newwin(SQUARE_HEIGHT, SQUARE_WIDTH, terminal_h / 2 - 3, (terminal_w + 4) / 2);

	mvprintw(terminal_h / 2 - 8, (terminal_w - Board::Get().GetLevelEndMessage().length()) / 2,
			 Board::Get().GetLevelEndMessage().c_str());
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
		Camera::Get().DrawEntity(*ve, win_item_display);
		wrefresh(win_item_display);

		score += score_inc;
	}

	mvprintw(terminal_h / 2 - 6, (terminal_w - 14) / 2 + 7, (std::to_string(score) + "   ").c_str());
	refresh();
	sleep(2);

	delwin(win_item_display);
	sleep(1);

	Debugger::Get().Log("Score at the end of the level: " + std::to_string(score) + " .");
}

// Erases the screen, decreases score by 100, prints death_message
void level_restart_subroutine(int &score, const int terminal_w, const int terminal_h)
{
	Debugger::Get().Log("Restarting the level.");

	erase();

	mvprintw(terminal_h / 2 - 8, (terminal_w - Board::Get().GetDeathMessage().length()) / 2,
			 Board::Get().GetDeathMessage().c_str());
	mvprintw(terminal_h / 2 - 6, (terminal_w - 14) / 2, ("score: " + std::to_string(score) + " -100").c_str());
	refresh();
	sleep(2);

	score -= 100;
	mvprintw(terminal_h / 2 - 6, (terminal_w - 14) / 2, ("score: " + std::to_string(score) + "        ").c_str());
	refresh();
	sleep(2);

	Debugger::Get().Log("Score after dying: " + std::to_string(score) + " .");
}

// Erases the screen and prints episode finish message, waits for getch() to end
void episode_end_subroutine(const int score, const int terminal_w, const int terminal_h)
{
	Debugger::Get().Log("Episode finished.");
	Debugger::Get().Log("Score at the end of the game: " + std::to_string(score) + " .");

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
int item_gain_subroutine(Item item, message &currentmes, int &flashlight_lvl)
{
	message testmes = item.targetmessage();

	if (Inventory::Get().AddItem(item) == 0)
	{
		Debugger::Get().Log("Collected item: " + item.get_name() + ".");

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
		return 0;
	}
	else
	{
		Debugger::Get().Log("Couldn't collect item: " + item.get_name() + " because of full inventory.");

		// if could not collect item, replace message text with a warning
		testmes.text = "Inventory full, didn't collect item";
		currentmes   = more_important_message(currentmes, testmes);
		return 1;
	}
}

// Drops item to the ground (a.k.a. adds said item to Board::Get().GroundItems() )
// Checks for special cases (flashlight, super flashlight)
// Updates testmes
void item_lose_subroutine(message &testmes, const int player_x, const int player_y, int &flashlight_lvl)
{
	Item dropped_item = Inventory::Get().DropItem(player_x, player_y);
	Board::Get().GroundItems().push_back(dropped_item);
	string dropped_item_name = dropped_item.get_name();

	Debugger::Get().Log("Dropped " + dropped_item_name + " on the ground.");
	testmes.text = "Dropped " + dropped_item_name + " on the ground";

	// checking for special items
	// flashlight
	if (dropped_item_name == _flashlight_name && !Inventory::Get().Has(_flashlight_name) && flashlight_lvl == 1)
	{
		testmes.text   = "You dropped your " + _flashlight_name + ", your field of view decreases";
		flashlight_lvl = 0;
	}
	// super flashlight
	if (dropped_item_name == _super_flashlight_name && !Inventory::Get().Has(_super_flashlight_name))
	{
		testmes.text = "You dropped your super flashlight, what a shame";
		if (Inventory::Get().Has(_flashlight_name))
			flashlight_lvl = 1;
		else
			flashlight_lvl = 0;
	}
}

// Takes vector of TileManipulator pointers and runs turnhandler on each
// May update currentmes
void handle_tilemanip_turnhandlers_subroutine(message &currentmes)
{
	message testmes;
	for (size_t i = 0; i < Board::Get().TileManipulators().size(); ++i)
	{
		testmes    = Board::Get().TileManipulators()[i]->turnhandler(Board::Get().GetBoard());
		currentmes = more_important_message(currentmes, testmes);
	}
}

// Renders pretty much everything, that includes wins_squares, win_inventory and win_messages !! but not win_debug !!
void main_action_render_subroutine(Player &player, message currentmes, const ActionMode action_mode,
								   const int flashlight_lvl)
{
	Camera::Get().SetPosition(player.get_x(), player.get_y());

	// Rendering wins_squares
	vector<VisibleEntity *> visibleEntities;

	//? visibleEntities: Items, Enemies, Player
	for (size_t i = 0; i < Board::Get().GroundItems().size(); ++i)
		visibleEntities.push_back(&Board::Get().GroundItems()[i]);

	if (action_mode == am_dead)  // if player is dead, he should be below the enemies
		visibleEntities.push_back(&player);

	for (size_t i = 0; i < Board::Get().Enemies().size(); ++i)
		visibleEntities.push_back(Board::Get().Enemies()[i]);

	if (action_mode != am_dead)  // if player is not dead, he should be displayed on top of enemies
		visibleEntities.push_back(&player);

	Camera::Get().DrawFlashlightCircle(flashlight_lvl, Board::Get().GetBoard(), Board::Get().GetBoardW(),
									   Board::Get().GetBoardH(), visibleEntities);
	// Drawing inventory
	Inventory::Get().DrawInventory(action_mode == am_inventory_drop);

	// Printing to win_messages
	Camera::Get().UpdateWinMessages(currentmes.text);
}

// Makes wins_squares shine bright, renders all wins_squares (if show_full_screen = true), then makes the screen go
// black so flashlight circle can be rendered
void screen_flash_subroutine(const bool show_full_screen, Player &player)
{
	Debugger::Get().Log("Screen flash begin.");

	vector<VisibleEntity *> visibleEntities;
	//? visibleEntities: Items, Enemies, Player
	for (size_t i = 0; i < Board::Get().GroundItems().size(); ++i)
		visibleEntities.push_back(&Board::Get().GroundItems()[i]);

	for (size_t i = 0; i < Board::Get().Enemies().size(); ++i)
		visibleEntities.push_back(*&Board::Get().Enemies()[i]);

	visibleEntities.push_back(&player);

	Camera::Get().EffectFlash(show_full_screen, Board::Get().GetBoard(), Board::Get().GetBoardW(),
							  Board::Get().GetBoardH(), visibleEntities);

	Debugger::Get().Log("Screen flash end.");
}

// Sets player params and action mode to dead related, runs through 10 iterations of Enemy and TileManipulator
// turnhandlers
void player_kill_subroutine(Player &player, const message mes, ActionMode &action_mode, const int flashlight_lvl)
{
	Debugger::Get().Log("Player died.");

	player.set_dead(true);
	player.set_graphic(entity_graphics[11]);
	player.set_color_pair(103);
	action_mode = am_dead;

	main_action_render_subroutine(player, mes, action_mode, flashlight_lvl);

	sleep(1);
	message m;
	for (int i = 0; i < 10; ++i)
	{
		// moving enemies
		for (size_t j = 0; j < Board::Get().Enemies().size();)
		{
			m = Board::Get().Enemies()[j]->turnhandler(
				Board::Get().GetBoard(), Board::Get().GetBoardW(), Board::Get().GetBoardH(), player,
				std::min(flashlight_lvl + 2, Camera::Get().GetSquaresH() / 2),
				std::min(flashlight_lvl + 2, Camera::Get().GetSquaresV() / 2), Board::Get().Enemies());

			if (m.text == "#deletemeplease" && m.priority == 11)
				Board::Get().Enemies().erase(Board::Get().Enemies().begin() + j);
			else
				++j;
		}
		// moving tile manipulators
		handle_tilemanip_turnhandlers_subroutine(m);

		main_action_render_subroutine(player, mes, action_mode, flashlight_lvl);

		usleep(400000);
	}
}

// Main game loop
// Returns: 0 if the loop should run again
//          1 if game should quit
//          2 if the game should go back to title screen
//      and 3 if player has been killed so all variables should be set to default and stuff
int main_action_loop_routine(Player &player, int &tilemanip_turns_freeze, int &enemy_turns_freeze, const bool &nodelay,
							 ActionMode &action_mode, int &flashlight_lvl, bool &invincible)
{
	if (!nodelay && action_mode != am_command)
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
		enemy_turns_freeze     = std::max(0, enemy_turns_freeze - 1);
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
					Inventory::Get().DecrementSelectedItem();
					break;
				case KEY_DOWN:
				case KEY_LEFT:
					Inventory::Get().IncrementSelectedItem();
					break;
				default:
					arrow_pressed = false;
			}
			break;

		case am_command:
		{
			string str = Camera::Get().GetLastMessageText();
			if (ch == 10)  // key enter - executing command
			{
				delete_end_spaces(str);

				Debugger::Get().Log("Executing command: " + str);

				if (str == ":q!" || str == ":wq" || str == ":exit" || str == ":quit")
					return 1;
				else if (str.substr(0, 6) == ":give ")
				{
					const string subject = str.substr(6, str.size() - 1);
					if (subject == _green_key_name)
						item_gain_subroutine(_green_key, currentmes, flashlight_lvl);

					else if (subject == _flashlight_name)
						item_gain_subroutine(_flashlight, currentmes, flashlight_lvl);

					else if (subject == _camera_name)
						item_gain_subroutine(_camera, currentmes, flashlight_lvl);

					else if (subject == _stopwatch_name)
						item_gain_subroutine(_stopwatch, currentmes, flashlight_lvl);

					else if (subject == _super_flashlight_name || subject == "super flashlight")
						item_gain_subroutine(_super_flashlight, currentmes, flashlight_lvl);

					else if (subject == _skeleton_key_name)
						item_gain_subroutine(_skeleton_key, currentmes, flashlight_lvl);

					else if (subject == _red_key_name)
						item_gain_subroutine(_red_key, currentmes, flashlight_lvl);

					else if (subject == _blue_gem_name)
						item_gain_subroutine(_blue_gem, currentmes, flashlight_lvl);

					else if (subject == _red_gem_name)
						item_gain_subroutine(_red_gem, currentmes, flashlight_lvl);

					else if (subject == _green_gem_name)
						item_gain_subroutine(_green_gem, currentmes, flashlight_lvl);

					else
						currentmes.text = ":spooky error 16 - no item \"" + subject + "\"";
				}
				else if (str == ":light up")
					flashlight_lvl++;

				else if (str == ":light down")
					flashlight_lvl = std::max(flashlight_lvl - 1, 0);

				else if (str == ":invincible")
				{
					invincible      = !invincible;
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
					player_kill_subroutine(player, m, action_mode, flashlight_lvl);
					return 3;
				}

				// if no valid command was inputted, and command line is not empty
				else if (str != "" && str != ":")
					currentmes.text = ":spooky error 15 - invalid command";

				action_mode = am_normal;
				curs_set(0);
			}
			else if (ch == KEY_BACKSPACE)  // erasing last character
			{
				currentmes.text = str.substr(0, str.size() - 1);
			}
			else  // inputting a character
			{
				testmes.text    = ch;
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
					player.move_target(0, -1, Board::Get().GetBoardW(), Board::Get().GetBoardH());
					break;
				case KEY_DOWN:
					player.move_target(0, 1, Board::Get().GetBoardW(), Board::Get().GetBoardH());
					break;
				case KEY_LEFT:
					player.move_target(-1, 0, Board::Get().GetBoardW(), Board::Get().GetBoardH());
					break;
				case KEY_RIGHT:
					player.move_target(1, 0, Board::Get().GetBoardW(), Board::Get().GetBoardH());
					break;
				case ' ':
					// skipping turn (debug only)
					if (Debugger::Get().GetDebugEnabled())
						break;
				default:
					arrow_pressed = false;
					tilemanip_turns_freeze++;
					enemy_turns_freeze++;
			}

			// Checking if we can pass through the tiles
			if (arrow_pressed && Board::Get().GetBoard()[t_y][t_x].is_passable())
			{
				player.set_position(t_x, t_y);
			}

			// Getting message from tile, setting as new default
			testmes.text     = tile_messages[Board::Get().GetBoard()[t_y][t_x].get_message_num()];
			testmes.priority = 7;
			currentmes       = more_important_message(currentmes, testmes);
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
				Inventory::Get().IncrementPage();
				break;
			case KEY_PPAGE:
				Inventory::Get().DecrementPage();
				break;

			case 'd':
			case 'D':
				if (action_mode != am_inventory_drop && !Inventory::Get().IsEmpty())
				{
					action_mode = am_inventory_drop;

					testmes.text     = "Select item do drop, confirm with Enter or return with D";
					testmes.priority = 7;
					currentmes       = more_important_message(currentmes, testmes);

					Inventory::Get().SetSelectedItem(0);
				}
				else if (Inventory::Get().IsEmpty())
				{
					testmes.text     = "You have no items, and nothing to drop";
					testmes.priority = 7;
					currentmes       = more_important_message(currentmes, testmes);
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
					if (Inventory::Get().UseItem(_camera_name, false) == 0)
					{
						Debugger::Get().Log("Player used camera.");

						enemy_turns_freeze += 3;
						Camera::Get().UpdateWinMessages("Say cheese!");
						screen_flash_subroutine(true, player);
					}
					else if (Inventory::Get().Has(_camera_name))
						testmes.text = "Your camera isn't ready.";
					else
						testmes.text = "You don't have a Camera::Get().";

					currentmes = more_important_message(currentmes, testmes);
					break;
				}
			// stopwatch
			case 's':
			case 'S':
				if (action_mode == am_normal)
				{
					testmes.priority = 7;
					if (Inventory::Get().UseItem(_stopwatch_name, false) == 0)
					{
						Debugger::Get().Log("Player used stopwatch.");

						enemy_turns_freeze += 2;
						tilemanip_turns_freeze += 2;
						Camera::Get().UpdateWinMessages("Suddenly, everything around you freezes...");
						screen_flash_subroutine(false, player);
					}
					else if (Inventory::Get().Has(_camera_name))
						testmes.text = "Your stopwatch isn't ready.";
					else
						testmes.text = "You don't have a stopwatch.";

					currentmes = more_important_message(currentmes, testmes);
					break;
				}

			case 10:  // enter key
				if (action_mode == am_inventory_drop)
				{
					item_lose_subroutine(testmes, player.get_x(), player.get_y(), flashlight_lvl);

					testmes.priority = 7;
					currentmes       = more_important_message(currentmes, testmes);

					action_mode = am_normal;
					enemy_turns_freeze--;
					tilemanip_turns_freeze--;
				}
				else
					return 0;
				break;

			case (KEY_F0 + 7):
				if (Camera::Get().GetLastMessageText() == "Press F7 again to exit back to title screen")
				{
					Debugger::Get().Log("Exiting to title screen by F7.");
					return 2;
				}
				Camera::Get().UpdateWinMessages("Press F7 again to exit back to title screen");
				return 0;

			case (KEY_F0 + 10):
				if (Camera::Get().GetLastMessageText() == "Press F10 again to quit game without saving")
				{
					Debugger::Get().Log("Exiting the game by F10.");
					return 1;
				}
				Camera::Get().UpdateWinMessages("Press F10 again to quit game without saving");
				return 0;

			// command mode
			case ':':
				if (Debugger::Get().GetDebugEnabled() && action_mode == am_normal)
				{
					currentmes.priority = 1;
					currentmes.text     = ":";
					action_mode         = am_command;
					curs_set(2);
				}
				else
					return 0;
				break;

			default:
				return 0;
		}
	}

	Debugger::Get().DebugTimeStart();

	//* TileManipulator, Sign and Item targethandler section
	if (arrow_pressed && action_mode == am_normal)
	{
		// TileManipulators
		for (size_t i = 0; i < Board::Get().TileManipulators().size(); ++i)
		{
			if (Board::Get().TileManipulators()[i]->get_x() == t_x &&
				Board::Get().TileManipulators()[i]->get_y() == t_y)
			{
				testmes = Board::Get().TileManipulators()[i]->targethandler(
					Board::Get().GetBoard(), Board::Get().GetBoardH(), Board::Get().GetBoardW());
				currentmes = more_important_message(currentmes, testmes);
			}
		}

		// Signs
		for (auto it = begin(Board::Get().Signs()); it != end(Board::Get().Signs()); ++it)
		{
			if (it->get_x() == t_x && it->get_y() == t_y)
			{
				testmes    = it->targetmessage();
				currentmes = more_important_message(currentmes, testmes);
			}
		}

		// Items
		for (auto it = begin(Board::Get().GroundItems()); it != end(Board::Get().GroundItems());)
		{
			if (it->get_x() == t_x && it->get_y() == t_y)
			{
				if (!item_gain_subroutine(*it, currentmes, flashlight_lvl))
					it = Board::Get().GroundItems().erase(it);
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
		message testmes;

		for (size_t i = 0; i < Board::Get().Enemies().size();)
		{
			// Checking for if the enemy should kill us before moving
			if (Board::Get().Enemies()[i]->get_x() == player.get_x() &&
				Board::Get().Enemies()[i]->get_y() == player.get_y() && !invincible)
			{
				player_kill_subroutine(player, Board::Get().Enemies()[i]->targetmessage(), action_mode, flashlight_lvl);
				return 3;
			}

			testmes = Board::Get().Enemies()[i]->turnhandler(
				Board::Get().GetBoard(), Board::Get().GetBoardW(), Board::Get().GetBoardH(), player,
				std::min(flashlight_lvl + 2, Camera::Get().GetSquaresH() / 2),
				std::min(flashlight_lvl + 2, Camera::Get().GetSquaresV() / 2), Board::Get().Enemies());

			if (testmes.text == "#deletemeplease" && testmes.priority == 11)
			{
				Board::Get().Enemies().erase(Board::Get().Enemies().begin() + i);
			}
			else
			{
				// checking if enemy collided with player after moving
				if (Board::Get().Enemies()[i]->get_x() == player.get_x() &&
					Board::Get().Enemies()[i]->get_y() == player.get_y() && !invincible)
				{
					player_kill_subroutine(player, Board::Get().Enemies()[i]->targetmessage(), action_mode,
										   flashlight_lvl);
					return 3;
				}

				currentmes = more_important_message(currentmes, testmes);

				++i;
			}
		}
	}

	//* TileManipulator turnhandler section
	if (tilemanip_turns_freeze == 0 && action_mode == am_normal)
		handle_tilemanip_turnhandlers_subroutine(currentmes);

	//* Increment power of items
	if (arrow_pressed && action_mode == am_normal)
		Inventory::Get().IncrementPowerOfItems();

	auto debug_elapsed_time = Debugger::Get().DebugTimeGetElapsedTime();

	if (action_mode != am_command)
	{
		Debugger::Get().Log("Player at x:" + std::to_string(player.get_x()) + " y:" + std::to_string(player.get_y()) +
							", targeting x:" + std::to_string(t_x) + " y:" + std::to_string(t_y) +
							".\t\tProcessing time: " + std::to_string(debug_elapsed_time));
		if (currentmes.text != "")
			Debugger::Get().Log("Message displayed: " + currentmes.text + "");
	}

	//* Render section

	if (Debugger::Get().GetDebugEnabled())

		Camera::Get().UpdateWinDebug(debug_elapsed_time, ch, action_mode, tilemanip_turns_freeze, enemy_turns_freeze,
									 flashlight_lvl, invincible);

	main_action_render_subroutine(player, currentmes, action_mode, flashlight_lvl);

	//* Checking if we've reached the exit
	if (t_x == Board::Get().GetExitX() && t_y == Board::Get().GetExitY())
	{
		Debugger::Get().Log("Player reached exit.");
		usleep(200000);
		return -1;
	}

	return 0;
}
