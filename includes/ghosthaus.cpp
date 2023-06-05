#include "ghosthaus.hpp"

using std::string;

int ghosthaus(int argc, char const *argv[])
{
	//* INIT PROGRAM

	if (setup_ncurses_routine())
		return 0;

	bool debug;
	bool nodelay;
	string episode_path;
	int inventory_size;
	int _COLOR_GRAY = COLOR_BLACK;

	find_flags(argc, argv, debug, nodelay);

	if (debug)
		Debugger::Get().EnableDebug();

	srand((unsigned)time(NULL));

	Debugger::Get().Log("Program initialization done.");

title_screen:
	//* TITLE SCREEN SCENE
	if (scene_title_screen(nodelay, _COLOR_GRAY))
		return 0;

	//* EPISODE SELECT SCENE
	switch (scene_episode_select(episode_path, inventory_size))
	{
		case 1:
			return 0;
		case -1:
			goto title_screen;
		default:
			break;  // go further
	}

	int score = 0;

	// making sure episode path is proper with all the '/' characters
	delete_end_spaces(episode_path);

	if (episode_path.front() != '/')
		episode_path.insert(0, "/");

	if (episode_path.back() != '/')
		episode_path.push_back('/');

	for (int level = 1;; ++level)
	{
		//* MAIN GAME ACTION SCENE
		Inventory::Get().SetMaxItems(inventory_size);

		int main_action_return_val = scene_main_game_action(
			nodelay, "episodes" + episode_path + std::to_string(level) + ".txt", level, _COLOR_GRAY, score);

		if (main_action_return_val == 1)
		{
			if (level == 1)
				return 0;
			else
				break;
		}
		else if (main_action_return_val == 2)
			goto title_screen;
	}

	episode_end_subroutine(score, getmaxx(stdscr), getmaxy(stdscr));

	goto title_screen;

	endwin();
	return 0;
}