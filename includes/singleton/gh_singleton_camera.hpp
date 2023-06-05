#pragma once

#include <ncurses.h>
#include <unistd.h>

#include <vector>

#include "../class/gh_class_tile.hpp"
#include "../class/gh_class_visibleentity.hpp"
#include "../logic/gh_constants_and_colors.hpp"
#include "../logic/gh_enum_action_mode.hpp"
#include "../logic/gh_simple_functions.hpp"

using std::vector;

// Constains render related functions
class Camera
{
public:
	Camera(const Camera &) = delete;

	static Camera &Get()
	{
		static Camera INSTANCE;
		return INSTANCE;
	}

	WINDOW *&GetWinInventory();

	string GetLastMessageText() const;

	int GetSquaresH() const;

	int GetSquaresV() const;

	void SetPosition(int x, int y);

	void SetActionWindows(WINDOW *win_messages, WINDOW *win_inventory, WINDOW *win_main_border, WINDOW *win_debug,
						  vector<vector<WINDOW *>> wins_squares);

	void DeleteActionWindows();

	void BoxWinMainBorder(chtype arg1 = 0, chtype arg2 = 0);

	// Function updates window responsible for showing messages
	void UpdateWinMessages(const string &text);

	// Function updates the window that displays debug information
	void UpdateWinDebug(const long elapsed_time, const int last_char, ActionMode action_mode,
						const int tilemanip_turns_freeze, const int enemy_turns_freeze, const int flashlight_lvl,
						const bool invincible);

	// Function draws a single entity on a given single square
	void DrawEntity(VisibleEntity &visible_entity, WINDOW *&square);

	// Function turns all squares white, then renders all squares (if show_full_screen), then turns all squares black
	void EffectFlash(const bool show_full_screen, Tile **board, int board_w, int board_h,
					 vector<VisibleEntity *> &visible_entities);

	// Function renders screen (or rather updates windows in wins_squares).
	// It changes camera's x and y values if they're too close to the border,
	// so that it doesn't try to get invalid table indexes.
	void DrawFullScreen(Tile **board, const int board_w, const int board_h, vector<VisibleEntity *> &visible_entities);

	// Function turns all wins_squares black
	void DrawFullScreenDarkness();

	// Function turns all wins_squares white
	void DrawFullScreenFlash();

	// Function renders screen (updates wins_squares) around the center bounded
	// by a semi circle thingy (dependant on flashlight_lvl)
	// If flashlight level is lower than before, it also draws darkness around
	void DrawFlashlightCircle(int flashlight_lvl, Tile **board, const int board_w, const int board_h,
							  vector<VisibleEntity *> &visible_entities);

private:
	Camera() = default;

	int x;
	int y;
	string last_message_text;
	int last_flashlight_lvl = 0;

	WINDOW *win_messages;
	WINDOW *win_inventory;
	WINDOW *win_main_border;
	WINDOW *win_debug;
	vector<vector<WINDOW *>> wins_squares;

	int squares_h;
	int squares_v;

	// Restricts camera position so it's not too close to the map border
	void restrict_camera_pos(const int board_w, const int board_h)
	{
		restrict(x, squares_h / 2, board_w - squares_h / 2 - 1);
		restrict(y, squares_v / 2, board_h - squares_v / 2 - 1);
	}

	// Function draws tile and all visible_entities occupying given current_x and current_y
	void draw_square(const int current_x, const int current_y, vector<VisibleEntity *> &visible_entities,
					 Tile currentTile, WINDOW *&square)
	{
		int gr_id = currentTile.get_graphics_id();

		wbkgd(square, COLOR_PAIR(currentTile.get_color_pair()));

		// setting font to bold (gray when black) for rock and tombstone
		if (gr_id == 3 || gr_id == 4 || gr_id == 14)
		{
			wattron(square, A_BOLD);
		}
		else
		{
			wattroff(square, A_BOLD);
		}

		mvwprintw(square, 0, 0, tile_patterns[gr_id]);

		//* Drawing visible entities
		for (size_t i = 0; i < visible_entities.size(); ++i)
		{
			if (visible_entities[i]->get_x() == current_x && visible_entities[i]->get_y() == current_y)
			{
				DrawEntity(*visible_entities[i], square);
			}
		}
		wrefresh(square);
	}

	// Turns given square black
	void draw_black_square(WINDOW *&square)
	{
		werase(square);
		wbkgd(square, COLOR_PAIR(0));

		wrefresh(square);
	}

	// Fills given square with background color from color_pair
	void draw_colored_square(WINDOW *&square, const chtype color_pair)
	{
		werase(square);
		wbkgd(square, COLOR_PAIR(color_pair));
		wrefresh(square);
	}

	// Changes squares to black everywhere accept for the middle section
	// (dependant on flashlight_lvl)
	void draw_darkness_around(const int flashlight_lvl)
	{
		int val_1 = ((squares_v - 5) / 2) - flashlight_lvl;
		int val_2 = ((squares_h - 5) / 2) - flashlight_lvl;

		// top and bottom
		for (int j = 0; j < squares_h; ++j)
		{
			// top
			for (int i = 0; i < val_1; ++i) { draw_black_square(wins_squares[i][j]); }
			// bottom
			for (int i = std::max((squares_v - val_1), 0); i < squares_v; ++i)
			{ draw_black_square(wins_squares[i][j]); }
		}

		// left and right
		const int ibound = std::min((squares_v - val_1), squares_v);
		const int jbound = std::min(val_2, squares_h);
		for (int i = std::max(val_1, 0); i < ibound; ++i)
		{
			// left
			for (int j = 0; j < jbound; ++j) { draw_black_square(wins_squares[i][j]); }
			// right
			for (int j = std::max((squares_h - val_2), 0); j < squares_h; ++j)
			{ draw_black_square(wins_squares[i][j]); }
		}

		// corners
		if (val_1 >= 0)
		{
			draw_black_square(wins_squares[val_1][val_2]);
			draw_black_square(wins_squares[val_1][squares_h - val_2 - 1]);
			draw_black_square(wins_squares[squares_v - val_1 - 1][val_2]);
			draw_black_square(wins_squares[squares_v - val_1 - 1][squares_h - val_2 - 1]);
		}
	}
};
