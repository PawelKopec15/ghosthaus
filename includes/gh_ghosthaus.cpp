#include <ncurses.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "gh_ghosthaus.h"

#include "gh_game_scenes.h"
#include "gh_game_routines.h"
#include "gh_game_logic.h"

using std::string;

int ghosthaus(int argc, char const *argv[])
{
    //* INIT PROGRAM

    if (setup_ncurses_routine())
        return (0);

    // flags[0] - debug
    // flags[1] - no delay
    bool flags[2];
    string episode_path;
    int inventory_size;
    int _COLOR_GRAY = COLOR_BLACK;

    find_flags(argc, argv, flags);

    srand(time(NULL));

title_screen:
    //* TITLE SCREEN SCENE
    if (scene_title_screen(flags, _COLOR_GRAY))
        return (0);

    //* EPISODE SELECT SCENE
    switch (scene_episode_select(episode_path, inventory_size))
    {
    case 1:
        return (0);
    case -1:
        goto title_screen;
        break;
    }

    int score = 0;
    
    // making sure episode path is proper with all the /
    if (episode_path.substr(0, 1) != "/")
        episode_path = "/" + episode_path;
    if (episode_path.substr(episode_path.length() - 1, 1) != "/")
        episode_path += "/";

    for (int level = 1;; ++level)
    {
        //* MAIN GAME ACTION SCENE
        int main_action_return_val = scene_main_game_action(flags, "episodes" + episode_path + std::to_string(level) + ".txt", level, inventory_size, _COLOR_GRAY, score);
        if (main_action_return_val == 1)
        {
            if (level == 1)
                return (0);
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