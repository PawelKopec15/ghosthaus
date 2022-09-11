#pragma once

#include <iostream>
#include <vector>

#include "gh_class_tile.h"
#include "gh_class_positionobject.h"
#include "gh_class_visibleentity.h"
#include "gh_class_inventory.h"

using std::string;

//* TileManipulator -> extends PositionObject
// Constains functions: targethandler and turnhandler
class TileManipulator : public PositionObject
{
public:
    TileManipulator(int x, int y);

    // To be called when targeted by player
    virtual message targethandler(Tile **board, const int board_h, const int board_w, Inventory &inventory);

    // To be called every turn
    virtual message turnhandler(Tile **board);
};

//* Door -> extends TileManipulator
// Contains functionality necessary to operate door tiles, replacing them with opened and closed door when needed
// Locked 0 = unlocked door, 1 = green door, 2 = red door
class Door : public TileManipulator
{
private:
    int locked;
    int close_countdown = -1;
    Tile *closed_door = new Tile(false, 7, 16);
    Tile *opened_door = new Tile(true, 8, 17);
    message mes;

public:
    Door(int x, int y);
    Door(int x, int y, int locked);
    ~Door();

    message targethandler(Tile **board, const int board_h, const int board_w, Inventory &inventory);

    message turnhandler(Tile **board);
};

//* Switch -> extends TileManipulator
// Contains functionality of a switch that replaces switch walls with switch floors and vice versa
// Also changes colors of all switch object, so the player can know if a switch is active or not
class Switch : public TileManipulator
{
private:
    Tile *switch_wall = new Tile();
    Tile *switch_floor = new Tile();

public:
    Switch(int x, int y);
    ~Switch();

    message targethandler(Tile **board, const int board_h, const int board_w, Inventory &inventory);
};
