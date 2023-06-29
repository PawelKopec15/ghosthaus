#pragma once
#include <iostream>

#include "../class/gh_class_player.hpp"
#include "../class/gh_class_sign.hpp"
#include "../class/gh_class_tile.hpp"
#include "../class/gh_classes_enemies.hpp"
#include "../class/gh_classes_tilemanipulators.hpp"

class Board
{
public:
	Board(const Board &) = delete;

	static Board &Get()
	{
		static Board INSTANCE;
		return INSTANCE;
	}

	Tile **&GetBoard() { return board; }

	void ClearAllVectors()
	{
		std::vector<TileManipulator *> a;
		tileManipulators = a;

		std::vector<Enemy *> b;
		enemies = b;

		signs.clear();
		ground_items.clear();
	}

	void SetBoardDimensions(int width, int height)
	{
		this->board_h = height;
		this->board_w = width;
	}
	int GetBoardH() const { return board_h; }
	int GetBoardW() const { return board_w; }

	void SetFloorColor(int value) { this->_COLOR_FLOOR = value; }
	int GetFloorColor() const { return _COLOR_FLOOR; }

	void SetExitPosition(int x, int y)
	{
		this->exit_x = x;
		this->exit_y = y;
	}
	int GetExitX() const { return exit_x; }
	int GetExitY() const { return exit_y; }

	std::vector<TileManipulator *> &TileManipulators() { return tileManipulators; }
	std::vector<Enemy *> &Enemies() { return enemies; }
	std::vector<Sign> &Signs() { return signs; }
	std::vector<Item> &GroundItems() { return ground_items; }

	void SetLevelEndMessage(const string &message) { this->level_end_message = message; }
	string GetLevelEndMessage() const { return level_end_message; }

	void SetDeathMessage(const string &message) { this->death_message = message; }
	string GetDeathMessage() const { return death_message; }

private:
	Board() = default;

	string level_end_message;
	string death_message;

	Tile **board;

	int board_w;
	int board_h;

	int _COLOR_FLOOR;

	int exit_x = 0;
	int exit_y = 0;

	std::vector<TileManipulator *> tileManipulators;
	std::vector<Enemy *> enemies;
	std::vector<Sign> signs;
	std::vector<Item> ground_items;
};
