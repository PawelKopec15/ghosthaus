#include "gh_classes_tilemanipulators.hpp"

using std::string;

//* TileManipulator -> extends PositionObject
// Constains functions: targethandler and turnhandler

TileManipulator::TileManipulator(int x, int y) : PositionObject(x, y) {}
TileManipulator::~TileManipulator() = default;

// To be called when targeted by player
message TileManipulator::targethandler(Tile **board, const int board_h, const int board_w)
{
	message mes;
	mes.text     = "";
	mes.priority = 10;
	return mes;
};
// To be called every turn
message TileManipulator::turnhandler(Tile **board)
{
	message mes;
	mes.text     = "";
	mes.priority = 10;
	return mes;
};

//* Door -> extends TileManipulator
// Contains functionality necessary to operate door tiles, replacing them with opened and closed door when needed
// Locked 0 = unlocked door, 1 = green door, 2 = red door

Door::Door(int x, int y) : TileManipulator(x, y) { this->locked = 0; }
Door::Door(int x, int y, int locked) : TileManipulator(x, y) { this->locked = locked; }
Door::~Door()
{
	delete closed_door;
	delete opened_door;
}

message Door::targethandler(Tile **board, const int board_h, const int board_w)
{
	mes.priority = 3;
	mes.text     = "";
	// If door is idle
	if (close_countdown == -1)
	{
		if (locked == 0)
		{
			close_countdown = 3;
			board[y][x]     = *opened_door;
			mes.text        = "The door cracks open";
		}
		else
		{
			mes.text     = "The door is locked now";
			bool had_key = false;

			// if player has skeleton key unlock the door
			if (Inventory::Get().UseItem(_skeleton_key_name, false) == 0)
				had_key = true;
			else
			{
				// if player has a matching key unlock the door and destroy his key
				switch (locked)
				{
					case 1:
						if (Inventory::Get().UseItem(_green_key_name, true) == 0)
							had_key = true;
						break;
					case 2:
						if (Inventory::Get().UseItem(_red_key_name, true) == 0)
							had_key = true;
						break;
				}
			}

			if (had_key)
			{
				board[y][x] = *closed_door;
				locked      = 0;
				mes.text    = "You unlocked the door";
			}
		}
	}
	return mes;
}
message Door::turnhandler(Tile **board)
{
	mes.priority = 6;
	mes.text     = "";
	if (close_countdown > -1)
	{
		close_countdown--;
	}
	if (close_countdown == 0)  // close the door
	{
		board[y][x]     = *closed_door;
		close_countdown = -1;
		mes.text        = "The door closes behind you";
	}
	return mes;
}

//* Switch -> extends TileManipulator
// Contains functionality of a switch that replaces switch walls with switch floors and vice versa
// Also changes colors of all switch object, so the player can know if a switch is active or not

Switch::Switch(int x, int y) : TileManipulator(x, y)
{
	Tile *sw          = new Tile(false, 5, 24, 6);
	this->switch_wall = sw;

	Tile *sf           = new Tile(true, 16, 12, 6);
	this->switch_floor = sf;
}
Switch::~Switch()
{
	delete switch_wall;
	delete switch_floor;
}

message Switch::targethandler(Tile **board, const int board_h, const int board_w)
{
	message mes;
	bool switched_anything = false;
	mes.priority           = 3;
	mes.text               = "You pressed the switch, but nothing happened.";

	for (int i = 0; i < board_h; ++i)
	{
		for (int j = 0; j < board_w; ++j)
		{
			if (board[i][j].get_message_num() == 6)
			{
				switched_anything = true;

				// switch wall with floor
				if (board[i][j] == *switch_wall)
					board[i][j] = *switch_floor;

				// switch floor with wall
				else if (board[i][j] == *switch_floor)
					board[i][j] = *switch_wall;
			}
			else if (board[i][j].get_graphics_id() == 15)  // if it is a switch change color
			{
				if (board[i][j].get_color_pair() == 11)
					board[i][j].set_color_pair(12);
				else if (board[i][j].get_color_pair() == 12)
					board[i][j].set_color_pair(11);
			}
		}
	}

	if (switched_anything)
		mes.text = "You pressed the switch, you hear walls shifting...";

	return mes;
}
