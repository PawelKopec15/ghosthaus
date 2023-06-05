#include "gh_game_logic.hpp"

using std::ifstream;
using std::string;
using std::vector;
using std::filesystem::directory_iterator;

// Checks if any flags have been inputted during program execution
// Updates bool values representing given flags
void find_flags(int argc, char const *argv[], bool &debug, bool &nodelay)
{
	for (int i = 1; i < argc; ++i)
	{
		if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "-debug"))
			debug = true;
		if (!strcmp(argv[i], "-nd") || !strcmp(argv[i], "-nodelay"))
			nodelay = true;
	}
}

// Returns vector of .ep file paths in given folder
vector<string> find_ep_files(const string &path)
{
	vector<string> toRet;
	const string file_extention = ".ep";

	try
	{
		for (const auto &file : directory_iterator(path))
		{
			const string str = file.path().string();
			if (str.length() < file_extention.length())
				continue;
			if (str.compare(str.length() - file_extention.length(), file_extention.length(), file_extention) == 0)
				toRet.push_back(str);
		}
		return toRet;
	}
	catch (...)
	{
		vector<string> v;
		return v;
	}
}

// First line is line 1 !!!
string get_line_from_file(const string &path, const unsigned int lineNum = 0)
{
	std::ifstream file(path);
	std::string line;
	unsigned int currentLine = 0;

	while (std::getline(file, line))
	{
		++currentLine;
		if (currentLine == lineNum)
		{
			file.close();
			return line;
		}
	}

	file.close();
	return "";  // Empty string if lineNum is out of range
}

// This function returns a two dimensional pointer of Tile objects and updates
// board_w and board_h. Also adds entity objects to their respective vectors,
// updates player and exit position. Also updates level_end_message and
// death_message
void make_board(const string &file_path, const int level_num, Player &player)
{
	string line;
	ifstream InputFile(file_path);

	if (!InputFile)
	{
		if (level_num == 1)
		{
			Debugger::Get().LogErr("In make_board function, error creating the board.");
			Debugger::Get().LogErr("Error opening file \"" + file_path + "\".");
			const string error_mes = "Error opening file " + file_path + ".\n";
			printw(error_mes.c_str());
			refresh();
		}
		return;
	}

	int width            = 0;
	int height           = 0;
	bool found_data      = false;
	string level_end_mes = "Level complete!";
	string death_mes     = "Level restart";

	string sign_messages[9] = {"", "", "", "", "", "", "", "", ""};

	int ghost_level               = 1;
	string guard_kill_messages[4] = {"Wham! You are dead!", "Wham! You are done for!",
									 "Wham! You didn't stand a chance!", "Wham! You are unalived!"};
	int guard_nums[4][3]          = {{-1, 0, 0}, {0, -1, 0}, {-1, 0, 2}, {1, 0, 1}};

	int floor_color = COLOR_BLUE;

	while (!InputFile.eof())
	{
		getline(InputFile, line);
		delete_end_spaces(line);

		// it's really irritating there isn't a switch statement for strings in
		// c++ width and height of the board
		if (line == "#width")
		{
			try
			{
				getline(InputFile, line);
				delete_end_spaces(line);
				width = stoi(line);
			}
			catch (...)
			{
				continue;
			}
		}
		else if (line == "#height")
		{
			try
			{
				getline(InputFile, line);
				delete_end_spaces(line);
				height = stoi(line);
			}
			catch (...)
			{
				continue;
			}
		}
		// signs
		else if (line.substr(0, 6) == "#sign ")
		{
			int signnum = 0;
			try
			{
				signnum = stoi(line.substr(6, 1));
			}
			catch (...)
			{
				continue;
			}

			try
			{
				getline(InputFile, line);
				delete_end_spaces(line);

				sign_messages[signnum - 1] = line;
			}
			catch (...)
			{
				continue;
			}
		}
		// ghost level
		else if (line == "#ghost" || line == "#ghost_level" || line == "#ghost_lvl")
		{
			try
			{
				getline(InputFile, line);
				delete_end_spaces(line);
				ghost_level = stoi(line);
			}
			catch (...)
			{
				continue;
			}
		}
		// guard kill message
		else if (line.substr(0, 19) == "#guard_killmessage ")
		{
			int guard_i = 0;
			try
			{
				guard_i = stoi(line.substr(19, 1));
			}
			catch (...)
			{
				continue;
			}

			try
			{
				getline(InputFile, line);
				delete_end_spaces(line);

				guard_kill_messages[guard_i - 1] = line;
			}
			catch (...)
			{
				continue;
			}
		}
		// guard direction
		else if (line.substr(0, 17) == "#guard_direction ")
		{
			int guard_i = 0;
			try
			{
				guard_i = stoi(line.substr(17, 1));
			}
			catch (...)
			{
				continue;
			}

			try
			{
				getline(InputFile, line);
				delete_end_spaces(line);

				guard_nums[guard_i - 1][0] = stoi(line);
			}
			catch (...)
			{
				continue;
			}
			try
			{
				getline(InputFile, line);
				delete_end_spaces(line);

				guard_nums[guard_i - 1][1] = stoi(line);
			}
			catch (...)
			{
				continue;
			}
		}
		// guard mode
		else if (line.substr(0, 12) == "#guard_mode ")
		{
			int guard_i = 0;
			try
			{
				guard_i = stoi(line.substr(12, 1));
			}
			catch (...)
			{
				continue;
			}

			try
			{
				getline(InputFile, line);
				delete_end_spaces(line);

				guard_nums[guard_i - 1][2] = stoi(line);
			}
			catch (...)
			{
				continue;
			}
		}
		// floor color
		else if (line == "#floor" || line == "#floor_color")
		{
			try
			{
				getline(InputFile, line);
				delete_end_spaces(line);

				if (line == "COLOR_BLUE" || line == "BLUE" || line == "blue" || line == "1")
					floor_color = COLOR_BLUE;
				else if (line == "COLOR_BLACK" || line == "BLACK" || line == "black" || line == "0")
					floor_color = COLOR_BLACK;
				else if (line == "COLOR_MAGENTA" || line == "MAGENTA" || line == "magenta" || line == "2")
					floor_color = COLOR_MAGENTA;
				else
					continue;
			}
			catch (...)
			{
				continue;
			}
		}
		// level end message
		else if (line == "#level_end_message" || line == "#end_message")
		{
			try
			{
				getline(InputFile, line);
				delete_end_spaces(line);
				level_end_mes = line;
			}
			catch (...)
			{
				continue;
			}
		}
		// level end message
		else if (line == "#death_message")
		{
			try
			{
				getline(InputFile, line);
				delete_end_spaces(line);
				death_mes = line;
			}
			catch (...)
			{
				continue;
			}
		}

		// skipping setup to go to tile data section
		else if (line == "#data" || line == "##data")
		{
			found_data = true;
			break;
		}
		// if nothing matches just skip, maybe it's an enter or some junk
		else
			continue;
	}

	// Error checks
	bool err = false;
	if (!found_data)
	{
		Debugger::Get().LogErr("In make_board function, error creating the board.");
		Debugger::Get().LogErr("Board data ( #data ) not found.");
		printw("Board data ( #data ) not found.\n");
		refresh();
		err = true;
	}
	if (width == 0)
	{
		Debugger::Get().LogErr("In make_board function, error creating the board.");
		Debugger::Get().LogErr("Board width not found.");
		printw("Board data width not found.\n");
		refresh();
		err = true;
	}
	else if (width < 0)
	{
		Debugger::Get().LogErr("In make_board function, error creating the board.");
		Debugger::Get().LogErr("Invalid board width (<0).");
		printw("Invalid board width (<0).\n");
		refresh();
		err = true;
	}
	if (height == 0)
	{
		Debugger::Get().LogErr("In make_board function, error creating the board.");
		Debugger::Get().LogErr("Board height not found.");
		printw("Board data height not found.\n");
		refresh();
		err = true;
	}
	else if (height < 0)
	{
		Debugger::Get().LogErr("In make_board function, error creating the board.");
		Debugger::Get().LogErr("Invalid board height (<0).");
		printw("Invalid board height (<0).\n");
		refresh();
		err = true;
	}
	if (err)
	{
		Debugger::Get().LogErr("Error occurred while reading file: \"" + file_path + "\".");
		printw("Invalid board width (<0).\n");
		printw("\nerror occurred while reading file:\n");
		printw(file_path.c_str());
		refresh();

		InputFile.close();
		return;
	}

	// Most used tiles
	const Tile TILE_FLOOR = Tile(true, 0, 11);
	const Tile TILE_PATH  = Tile(true, 1, 11);
	const Tile TILE_TABLE = Tile(false, 10, 15, 7);

	width  = std::max(Camera::Get().GetSquaresH(), width);
	height = std::max(Camera::Get().GetSquaresV(), height);

	Board::Get().SetBoardDimensions(width, height);
	Board::Get().SetFloorColor(floor_color);
	Board::Get().SetLevelEndMessage(level_end_mes);
	Board::Get().SetDeathMessage(death_mes);

	Tile **currentboard = 0;
	currentboard        = new Tile *[height];

	for (int i = 0; i < height; ++i)
	{
		try
		{
			getline(InputFile, line);
		}
		catch (...)
		{
			line = string(width, '0');
		}

		currentboard[i] = new Tile[width];

		for (int j = 0; j < width; ++j)
		{
			char c;
			try
			{
				c = line[j];
			}
			catch (...)
			{
				c = '0';
			}

			//* -----------
			//* MAIN SWITCH
			//* -----------

			switch (c)
			{
				//* Player / enemies
				case 'P':  // player
				{
					player.set_position(j, i);
					currentboard[i][j] = TILE_FLOOR;
					break;
				}
				case 'G':  // ghost
				{
					Enemy *en = new Ghost(j, i, entity_graphics[6], entity_graphics[7], 100,
										  "\'\'there is no escape\'\'", ghost_level);
					Board::Get().Enemies().push_back(en);
					currentboard[i][j] = TILE_FLOOR;
					break;
				}
				case 'a':  // guard 1
				{
					Enemy *en =
						new GuardEnemy(j, i, entity_graphics[4], entity_graphics[5], 103, guard_kill_messages[0],
									   guard_nums[0][0], guard_nums[0][1], guard_nums[0][2]);
					Board::Get().Enemies().push_back(en);

					currentboard[i][j] = TILE_PATH;
					break;
				}
				case 'b':  // guard 2
				{
					Enemy *en =
						new GuardEnemy(j, i, entity_graphics[5], entity_graphics[4], 103, guard_kill_messages[1],
									   guard_nums[1][0], guard_nums[1][1], guard_nums[1][2]);
					Board::Get().Enemies().push_back(en);

					currentboard[i][j] = TILE_PATH;
					break;
				}
				case 'c':  // guard 3
				{
					Enemy *en =
						new GuardEnemy(j, i, entity_graphics[4], entity_graphics[5], 101, guard_kill_messages[2],
									   guard_nums[2][0], guard_nums[2][1], guard_nums[2][2]);
					Board::Get().Enemies().push_back(en);

					currentboard[i][j] = TILE_PATH;
					break;
				}
				case 'd':  // guard 4
				{
					Enemy *en =
						new GuardEnemy(j, i, entity_graphics[5], entity_graphics[4], 101, guard_kill_messages[3],
									   guard_nums[3][0], guard_nums[3][1], guard_nums[3][2]);
					Board::Get().Enemies().push_back(en);

					currentboard[i][j] = TILE_PATH;
					break;
				}

				//* Passthrough
				case ' ':  // floor
				{
					currentboard[i][j] = TILE_FLOOR;
					break;
				}
				case '.':  // path
				{
					currentboard[i][j] = TILE_PATH;
					break;
				}
				case ',':  // grass
				{
					currentboard[i][j] = Tile(true, 2, 12);
					break;
				}

				//* Not passthrough
				case '#':  // brick wall
				{
					currentboard[i][j] = Tile(false, 5, 14);
					break;
				}
				case 'p':  // pillar
				{
					currentboard[i][j] = Tile(false, 14, 13);
					break;
				}

				//* Not passthrough with message
				case 'r':  // rock
				{
					currentboard[i][j] = Tile(false, 3, 13, 2);
					break;
				}
				case '+':  // grave
				{
					currentboard[i][j] = Tile(false, 4, 13, 3);
					break;
				}
				case 'w':  // water
				{
					currentboard[i][j] = Tile(false, 9, 18, 4);
					break;
				}
				case '~':  // red water
				{
					currentboard[i][j] = Tile(false, 9, 19);
					break;
				}
				case 't':  // table
				{
					currentboard[i][j] = TILE_TABLE;
					break;
				}

				//* Fake walls
				case '$':  // fake brick wall
				{
					currentboard[i][j] = Tile(true, 5, 14);
					break;
				}
				case 'f':  // fake rock
				{
					currentboard[i][j] = Tile(true, 3, 13);
					break;
				}

				//* Things that also modify tiles above them
				case '`':  // floor and table above
				{
					if (i > 0)
						currentboard[i - 1][j] = TILE_TABLE;
					currentboard[i][j] = TILE_FLOOR;
					break;
				}
				case 'T':  // tree and upper tree above
				{
					if (i > 0)
						currentboard[i - 1][j] = Tile(false, 11, 12);
					currentboard[i][j] = Tile(false, 12, 12);
					break;
				}

				//* Items
				case 'A':  // skeleton (almighty) key
				{
					Item it = _skeleton_key;
					it.set_position(j, i);
					Board::Get().GroundItems().push_back(it);

					currentboard[i][j] = TILE_FLOOR;
					break;
				}
				case 'K':  // green key
				{
					Item it = _green_key;
					it.set_position(j, i);
					Board::Get().GroundItems().push_back(it);

					currentboard[i][j] = TILE_FLOOR;
					break;
				}
				case 'L':  // red key
				{
					Item it = _red_key;
					it.set_position(j, i);
					Board::Get().GroundItems().push_back(it);

					currentboard[i][j] = TILE_FLOOR;
					break;
				}
				case 'F':  // flashlight
				{
					Item it = _flashlight;
					it.set_position(j, i);
					Board::Get().GroundItems().push_back(it);

					currentboard[i][j] = TILE_FLOOR;
					break;
				}
				case 'S':  // stopwatch
				{
					Item it = _stopwatch;
					it.set_position(j, i);
					Board::Get().GroundItems().push_back(it);

					currentboard[i][j] = TILE_FLOOR;
					break;
				}
				case 'C':  // camera
				{
					Item it = _camera;
					it.set_position(j, i);
					Board::Get().GroundItems().push_back(it);

					currentboard[i][j] = TILE_FLOOR;
					break;
				}
				case 'g':  // blue gemstone
				{
					Item it = _blue_gem;
					it.set_position(j, i);
					Board::Get().GroundItems().push_back(it);

					currentboard[i][j] = TILE_FLOOR;
					break;
				}
				case 'h':  // red gemstone
				{
					Item it = _red_gem;
					it.set_position(j, i);
					Board::Get().GroundItems().push_back(it);

					currentboard[i][j] = TILE_FLOOR;
					break;
				}
				case 'i':  // green gemstone
				{
					Item it = _green_gem;
					it.set_position(j, i);
					Board::Get().GroundItems().push_back(it);

					currentboard[i][j] = TILE_FLOOR;
					break;
				}

				//* Doors and exit
				case 'D':  // unlocked door
				{
					currentboard[i][j] = Tile(false, 7, 16);

					TileManipulator *door = new Door(j, i, 0);
					Board::Get().TileManipulators().push_back(door);

					break;
				}
				case 'M':  // green door (locked)
				{
					currentboard[i][j] = Tile(false, 7, 20);

					TileManipulator *door = new Door(j, i, 1);
					Board::Get().TileManipulators().push_back(door);
					break;
				}
				case 'N':  // red door (locked)
				{
					currentboard[i][j] = Tile(false, 7, 23);

					TileManipulator *door = new Door(j, i, 2);
					Board::Get().TileManipulators().push_back(door);
					break;
				}
				case 'E':  // exit
				{
					currentboard[i][j] = Tile(true, 13, 22);
					Board::Get().SetExitPosition(j, i);
					break;
				}

				//* Switch and co
				case '!':  // switch
				{
					currentboard[i][j] = Tile(false, 15, 11);

					TileManipulator *s = new Switch(j, i);
					Board::Get().TileManipulators().push_back(s);

					break;
				}
				case '@':  // switchable wall
				{
					currentboard[i][j] = Tile(false, 5, 24, 6);
					break;
				}
				case '_':  // switchable floor
				{
					currentboard[i][j] = Tile(true, 16, 12, 6);
					break;
				}

				//* Signs
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					const int num = c - '0';
					Board::Get().Signs().emplace_back(j, i, sign_messages[num - 1]);

					currentboard[i][j] = Tile(false, 6, 21);  // sign tile
					break;
				}

				//* Void
				case '0':
				default:
					currentboard[i][j] = Tile();  // defaults to void
					break;
			}
		}
	}

	Board::Get().GetBoard() = currentboard;

	InputFile.close();
}