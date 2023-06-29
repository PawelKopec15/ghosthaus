#include "gh_game_scenes.hpp"

using std::vector;

// Creates all windows for title screen, has all the title screen logic and destroys windows at the
// end returns 0 if option to start a new game has been selected returns 1 if terminal is smaller
// than 80x24 or game should end
int scene_title_screen(const bool &nodelay, int &_COLOR_GRAY)
{
	Debugger::Get().Log("Entering title screen scene.");

	curs_set(1);
	curs_set(0);
	int terminal_w;
	int terminal_h;

	if (verify_terminal_size_routine(terminal_w, terminal_h))
		return 1;

	int selected_option = 1;
	int page            = 0;
	WINDOW *win_logo;
	WINDOW *win_options;

	erase();
	mvprintw(terminal_h - 2, terminal_w - 10, "ver 1.1");
	refresh();

	setup_title_screen_windows(win_logo, win_options, terminal_w, terminal_h);

	if (Debugger::Get().GetDebugEnabled())
		mvprintw(terminal_h - 2, 0, "Debug enabled");
	if (nodelay)
		mvprintw(terminal_h - 1, 0, "No delay enabled");

	while (true)
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
							return 0;
						case 3:
							page = 2;
							break;
						case 4:
							delwin(win_logo);
							delwin(win_options);
							endwin();
							return 1;
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

// Creates all windows for episode select, has all the episode select logic and destroys windows at
// the end returns 0 if episode has been selected returns 1 if terminal is smaller than 80x24
// returns -1 if option to go back has been pressed
int scene_episode_select(string &episode_path, int &inventory_size)
{
	Debugger::Get().Log("Entering episode select scene.");

	curs_set(0);
	int terminal_w, terminal_h;
	if (verify_terminal_size_routine(terminal_w, terminal_h))
		return 1;

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
		return -1;
	}

	const unsigned int page_height = getmaxy(win_episode_select) - 5;
	const int page_count           = get_page_count(episode_file_paths.size(), page_height);

	int selected_option = 1;
	int page            = 0;

	while (true)
	{
		unsigned int main_return_val = episode_select_loop_routine(selected_option, page, page_count, page_height,
																   win_episode_select, episode_file_paths);
		if (main_return_val == page_height + 1)
		{
			delwin(win_episode_select);
			return -1;
		}
		else if (main_return_val > 0 && main_return_val <= episode_file_paths.size())
		{
			episode_path = get_line_from_file(episode_file_paths[selected_option - 1 + page * page_height], 2);
			Debugger::Get().Log("Selected episode path: \"" + episode_path + "\".");
			inventory_size = stoi(get_line_from_file(episode_file_paths[selected_option - 1 + page * page_height], 3));

			new_game_messageprint_subroutine(episode_file_paths[selected_option - 1 + page * page_height], terminal_w,
											 terminal_h);

			return 0;
		}
	}
}

// Creates all windows for main game action, has all the main action logic and destroys windows at
// the end returns 1 if terminal is smaller than 80x24 or something else happened that should result
// in ending the program returns 2 if option to return to title screen has been selected
int scene_main_game_action(const bool &nodelay, const string &file_path, const int level_num, const int _COLOR_GRAY,
						   int &score)
{
	Debugger::Get().Log("Entering main game action scene.");
	Debugger::Get().Log("Level path: \"" + file_path + "\".");

	int terminal_w;
	int terminal_h;
	if (verify_terminal_size_routine(terminal_w, terminal_h))
		return 1;

	int margin_side;
	int margin_topbottom;

	setup_action_windows(terminal_w, terminal_h, margin_side, margin_topbottom);

	Debugger::Get().Log("Done setting up game windows.");

respawn_point:

	erase();
	refresh();

	Player player(0, 0, entity_graphics[0], 100);

	std::vector<Item> items;
	Inventory::Get().SetItems(items);
	Inventory::Get().SetPage(0);
	Inventory::Get().SetSelectedItem(0);

	int tilemanip_turns_freeze = 0;
	int enemy_turns_freeze     = 0;

	ActionMode action_mode = am_normal;

	int flashlight_lvl = 0;

	bool invincible = false;

	Board::Get().ClearAllVectors();
	make_board(file_path, level_num, player);

	if (Board::Get().GetBoard() == NULL)  // if error occurred
	{
		if (level_num == 1)
		{
			Camera::Get().DeleteActionWindows();
			flushinp();
			getch();
			endwin();
		}
		return 1;
	}

	Debugger::Get().Log("Success creating the level board.");

	Camera::Get().DrawFullScreenDarkness();

	init_game_pairs(_COLOR_GRAY, Board::Get().GetFloorColor());

	Camera::Get().BoxWinMainBorder('+');
	main_action_render_subroutine(player, message(), action_mode, flashlight_lvl);

	Debugger::Get().Log("Rendered for the first time.");

	while (true)
	{
		const int main_return_val = main_action_loop_routine(player, tilemanip_turns_freeze, enemy_turns_freeze, nodelay,
															 action_mode, flashlight_lvl, invincible);
		if (main_return_val == 1)
		{
			Camera::Get().DeleteActionWindows();
			endwin();
			return 1;
		}
		else if (main_return_val == 2)
		{
			Camera::Get().DeleteActionWindows();
			return 2;
		}
		else if (main_return_val == 3)
		{
			level_restart_subroutine(score, terminal_w, terminal_h);
			goto respawn_point;
		}
		else if (main_return_val == -1)
			break;
	}

	Camera::Get().DeleteActionWindows();

	//* Level end moment

	init_game_pairs(_COLOR_GRAY, COLOR_BLACK);
	level_end_subroutine(Inventory::Get().GetAllItems(), score, terminal_w, terminal_h);

	erase();
	refresh();
	usleep(500000);

	return 0;
}