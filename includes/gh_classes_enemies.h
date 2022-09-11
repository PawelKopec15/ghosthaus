#pragma once

#include <stdlib.h>
#include <math.h>
#include <vector>

#include "gh_struct_message.h"
#include "gh_simple_functions.h"
#include "gh_class_visibleentity.h"
#include "gh_class_tile.h"
#include "gh_class_player.h"

using std::vector;

//* Enemy -> extends VisibleEntity
// Class used to represent Enemy entities, contains string alt_graphic (that makes two graphic strings)
// Also string kill_message, function turnhandler and appropriate getters/setters
// bool display_alt it used in overriden get_graphic() function
class Enemy : public VisibleEntity
{
protected:
    string alt_graphic;
    message kill_message;

    bool display_alt;

public:
    Enemy(int x, int y, string graphic, string alt_graphic, char color_pair, string kill_message_text);

    virtual message turnhandler(Tile **board, const int board_w, const int board_h, Player player, const int fov_x, const int fov_y, vector<Enemy *> &enemies);

    virtual string get_graphic();

    message targetmessage();

    void set_alt_graphic(string alt_graphic);

    void set_kill_message_text(string kill_message_text);
};

//* GuardEnemy -> extends Enemy
// Simple enemy, walks forward in (dir_x, dir_y) direction until the next move would place him on a non passable tile or out of bounds
// When can't walk anymore, changes direction depending on mode variable
// mode = 0 (or default): reverse direction
// mode = 1: rotate direction clockwise
// mode = 2: rotate direction counter clockwise
class GuardEnemy : public Enemy
{

private:
    int dir_x, dir_y;
    unsigned char mode;

public:
    GuardEnemy(int x, int y, string graphic, string alt_graphic, char color_pair, string kill_message_text, int dir_x, int dir_y, unsigned char mode);

    virtual message turnhandler(Tile **board, const int board_w, const int board_h, Player player, const int fov_x, const int fov_y, vector<Enemy *> &enemies);
};

//* FakeGhost -> extends Enemy
// this enemy kills itself 1 turn after being created
class FakeGhost : public Enemy
{
private:
    bool time = false;

public:
    FakeGhost(int x, int y, string graphic, string alt_graphic, char color_pair, string kill_message_text);

    virtual message turnhandler(Tile **board, const int board_w, const int board_h, Player player, const int fov_x, const int fov_y, vector<Enemy *> &enemies);
};

//* Ghost -> extends Enemy
// Main enemy of the game, chases player in various (random) ways
// int craziness is responsible for frequency of interesting behaviors
class Ghost : public Enemy
{

private:
    unsigned int craziness;
    bool active = false;
    unsigned int special_counter = 0;
    int turns_doublestep_blocked = 5;
    int turns_shock_blocked = 13;

    template <typename T>
    int sgn(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    int sign(double x)
    {
        return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
    }

    // Dir:
    // 1 to follow
    // -1 to escape
    // 0 to do nothing
    void move_relative_to_player(const int p_x, const int p_y, int dir)
    {
        int dx = p_x - x;
        int dy = p_y - y;

        if (std::abs(dx) == std::abs(dy))
        {
            if (rand() % 2)
                ++dx;
            else
                ++dy;
        }

        double angle = std::atan2(dy, dx); // returns value between -pi and pi, or 0 if both args are 0
        angle += M_PI;

        double move_x = -std::cos(angle);
        double move_y = -std::sin(angle);

        if ((std::abs(dx) == 1 || std::abs(dy) == 1) && (std::abs(dx) != std::abs(dy)) && rand() % 4 == 0)
        {
            if (std::abs(move_x) < std::abs(move_y))
                x += sign(move_x) * dir;
            else
                y += sign(move_y) * dir;
        }
        else if (std::abs(move_x) > std::abs(move_y))
            x += sign(move_x) * dir;
        else
            y += sign(move_y) * dir;
    }

public:
    Ghost(int x, int y, string graphic, string alt_graphic, char color_pair, string kill_message_text, unsigned int craziness);

    virtual message turnhandler(Tile **board, const int board_w, const int board_h, Player player, const int fov_x, const int fov_y, vector<Enemy *> &enemies);

    string get_graphic();
};
