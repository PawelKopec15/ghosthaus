#include "gh_singleton_camera.hpp"

using std::vector;

// Constains render related functions

WINDOW *&Camera::GetWinInventory() { return win_inventory; }

string Camera::GetLastMessageText() const { return last_message_text; }

int Camera::GetSquaresH() const { return squares_h; }
int Camera::GetSquaresV() const { return squares_v; }

void Camera::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Camera::SetActionWindows(WINDOW *win_messages, WINDOW *win_inventory, WINDOW *win_main_border, WINDOW *win_debug,
							  vector<vector<WINDOW *>> wins_squares)
{
	this->win_messages    = win_messages;
	this->win_inventory   = win_inventory;
	this->win_main_border = win_main_border;
	this->win_debug       = win_debug;
	this->wins_squares    = wins_squares;

	this->squares_h = wins_squares[0].size();
	this->squares_v = wins_squares.size();

	box(this->win_main_border, 0, 0);
	wrefresh(this->win_main_border);
}

void Camera::DeleteActionWindows()
{
	delwin(this->win_messages);
	delwin(this->win_inventory);
	delwin(this->win_main_border);
	delwin(this->win_debug);
}

void Camera::BoxWinMainBorder(chtype arg1, chtype arg2)
{
	box(win_main_border, arg1, arg2);
	wrefresh(win_main_border);
}

// Function updates window responsible for showing messages
void Camera::UpdateWinMessages(const string &text)
{
	werase(win_messages);
	box(win_messages, 0, 0);

	const int h_text_position = (getmaxx(win_messages) / 2) - ((int)text.length() / 2);
	// const int h_text_position = 1;

	mvwprintw(win_messages, 1, h_text_position, text.c_str());
	last_message_text = text;
	wrefresh(win_messages);
}

// Function updates the window that displays debug information
void Camera::UpdateWinDebug(const long elapsed_time, const int last_char, ActionMode action_mode,
							const int tilemanip_turns_freeze, const int enemy_turns_freeze, const int flashlight_lvl,
							const bool invincible)
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
void Camera::DrawEntity(VisibleEntity &visible_entity, WINDOW *&square)
{
	// entity drawing, witch ignores white space
	wattron(square, A_BOLD);
	wattron(square, COLOR_PAIR(visible_entity.get_color_pair()));
	string entity_graphic = visible_entity.get_graphic();

	for (unsigned int c = 0; c < entity_graphic.length(); c++)  // haha c++
	{
		char cc = entity_graphic[c];
		if (cc == '\t')  // \t will be interpreted as actually drawing a space char
			mvwprintw(square, c / SQUARE_WIDTH, c - SQUARE_WIDTH * (c / SQUARE_WIDTH), " ");
		else if (cc != ' ')  // actual space will be ignored (used to iterate c)
		{
			const char ccc[2] = {cc, '\0'};  // necessary, as mvwprintw() function
											 // prints garbage if it can't find \0
			mvwprintw(square, c / SQUARE_WIDTH, c - SQUARE_WIDTH * (c / SQUARE_WIDTH), ccc);
		}
	}
}

// Function turns all squares white, then renders all squares (if show_full_screen), then turns all squares black
void Camera::EffectFlash(const bool show_full_screen, Tile **board, int board_w, int board_h,
						 vector<VisibleEntity *> &visible_entities)
{
	// white flash
	DrawFullScreenFlash();
	usleep(100000);
	if (show_full_screen)
	{
		// drawing full screen
		DrawFullScreen(board, board_w, board_h, visible_entities);
		usleep(1500000);
		// black screen
		DrawFullScreenDarkness();
	}
	else
		usleep(2000000);
	usleep(100000);
	// bugfix
	draw_darkness_around(last_flashlight_lvl);
}

// Function renders screen (or rather updates windows in wins_squares).
// It changes camera's x and y values if they're too close to the border,
// so that it doesn't try to get invalid table indexes.
void Camera::DrawFullScreen(Tile **board, const int board_w, const int board_h,
							vector<VisibleEntity *> &visible_entities)
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
			draw_square(current_x, current_y, visible_entities, currentTile, wins_squares[i][j]);
		}
	}
}

// Function turns all wins_squares black
void Camera::DrawFullScreenDarkness()
{
	for (int i = 0; i < squares_v; ++i)
	{
		for (int j = 0; j < squares_h; ++j)
		{
			draw_black_square(wins_squares[i][j]);
		}
	}
}

// Function turns all wins_squares white
void Camera::DrawFullScreenFlash()
{
	for (int i = 0; i < squares_v; ++i)
	{
		for (int j = 0; j < squares_h; ++j)
		{
			draw_colored_square(wins_squares[i][j], 8);
		}
	}
}

// Function renders screen (updates wins_squares) around the center bounded by a
// semi circle thingy (dependant on flashlight_lvl)
// If flashlight level is lower than before, it also draws darkness around
void Camera::DrawFlashlightCircle(int flashlight_lvl, Tile **board, const int board_w, const int board_h,
								  vector<VisibleEntity *> &visible_entities)
{
	restrict_camera_pos(board_w, board_h);

	int val_1 = ((squares_v - 5) / 2) - flashlight_lvl;
	int val_2 = ((squares_h - 5) / 2) - flashlight_lvl;

	//* Checking if flashlight level has been lowered
	if (last_flashlight_lvl > flashlight_lvl)
		draw_darkness_around(flashlight_lvl);
	last_flashlight_lvl = flashlight_lvl;

	//* Drawing tiles
	// top and bottom rows
	if (val_1 >= 0)
	{
		for (int j = val_2 + 1; j < (squares_h - val_2 - 1); ++j)
		{
			int current_y1 = y - squares_v / 2 + val_1;
			int current_y2 = y - squares_v / 2 + squares_v - val_1 - 1;
			int current_x  = x - squares_h / 2 + j;

			// better not to look at this code for too long
			draw_square(current_x, current_y1, visible_entities, board[current_y1][current_x], wins_squares[val_1][j]);
			draw_square(current_x, current_y2, visible_entities, board[current_y2][current_x],
						wins_squares[squares_v - val_1 - 1][j]);
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
			draw_square(current_x, current_y, visible_entities, currentTile, wins_squares[i][j]);
		}
	}
}
