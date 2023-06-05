#include "gh_classes_enemies.hpp"

using std::vector;

//* Enemy -> extends VisibleEntity
// Class used to represent Enemy entities, contains string alt_graphic (that makes two graphic strings)
// Also string kill_message, function turnhandler and appropriate getters/setters
// bool display_alt it used in overriden get_graphic() function

Enemy::Enemy(int x, int y, string graphic, string alt_graphic, char color_pair, string kill_message_text) : VisibleEntity(x, y, graphic, color_pair)
{
    this->alt_graphic = alt_graphic;

    this->kill_message.text = kill_message_text;
    kill_message.priority = 1;

    this->display_alt = false;
}
Enemy::~Enemy() = default;

message Enemy::turnhandler(Tile **board, const int board_w, const int board_h, Player player, const int fov_x, const int fov_y, vector<Enemy *> &enemies)
{
    message mes;
    mes.text = "";
    mes.priority = 10;
    return mes;
};

string Enemy::get_graphic()
{
    if (display_alt)
        return alt_graphic;
    else
        return graphic;
}

message Enemy::targetmessage()
{
    return kill_message;
}

void Enemy::set_alt_graphic(string alt_graphic)
{
    this->alt_graphic = alt_graphic;
}

void Enemy::set_kill_message_text(string kill_message_text)
{
    this->kill_message.text = kill_message_text;
}

//* GuardEnemy -> extends Enemy
// Simple enemy, walks forward in (dir_x, dir_y) direction until the next move would place him on a non passable tile or out of bounds
// When can't walk anymore, changes direction depending on mode variable
// mode = 0 (or default): reverse direction
// mode = 1: rotate direction clockwise
// mode = 2: rotate direction counter clockwise

GuardEnemy::GuardEnemy(int x, int y, string graphic, string alt_graphic, char color_pair, string kill_message_text, int dir_x, int dir_y, unsigned char mode) : Enemy(x, y, graphic, alt_graphic, color_pair, kill_message_text)
{
    this->dir_x = dir_x;
    this->dir_y = dir_y;
    this->mode = mode;
}

message GuardEnemy::turnhandler(Tile **board, const int board_w, const int board_h, Player player, const int fov_x, const int fov_y, vector<Enemy *> &enemies)
{

    // switch graphic
    display_alt = !display_alt;

    // if next tile in the way is passthrough and we're not out of bounds, go there
    if (board[y + dir_y][x + dir_x].is_passable() && y + dir_y >= 0 && x + dir_x >= 0 && (y + dir_y) < board_h && (x + dir_x) < board_w)
    {
        x += dir_x;
        y += dir_y;
    }
    else // else change direction, depending on mode
    {
        switch (mode)
        {
        case 1: // rotate clockwise
        {
            int a = dir_x;
            dir_x = -dir_y;
            dir_y = a;
            break;
        }
        case 2: // rotate counter clockwise
        {
            int a = dir_y;
            dir_y = -dir_x;
            dir_x = a;
            break;
        }
        case 0: // reverse direction
        default:
            dir_x *= (-1);
            dir_y *= (-1);
            break;
        }
    }

    // returning empty message, nothing ever happens here
    message mes;
    mes.text = "";
    mes.priority = 10;
    return mes;
};

//* FakeGhost -> extends Enemy
// this enemy kills itself 1 turn after being created

FakeGhost::FakeGhost(int x, int y, string graphic, string alt_graphic, char color_pair, string kill_message_text) : Enemy(x, y, graphic, alt_graphic, color_pair, kill_message_text) {}

message FakeGhost::turnhandler(Tile **board, const int board_w, const int board_h, Player player, const int fov_x, const int fov_y, vector<Enemy *> &enemies)
{
    message mes;
    if (time)
    {
        mes.text = "#deletemeplease";
        mes.priority = 11;
    }
    else
    {
        time = true;
        mes.text = "";
        mes.priority = 10;
    }
    return (mes);
}

//* Ghost -> extends Enemy
// Main enemy of the game, chases player in various (random) ways
// int craziness is responsible for frequency of interesting behaviors

Ghost::Ghost(int x, int y, string graphic, string alt_graphic, char color_pair, string kill_message_text, unsigned int craziness) : Enemy(x, y, graphic, alt_graphic, color_pair, kill_message_text)
{
    this->craziness = craziness;
}

message Ghost::turnhandler(Tile **board, const int board_w, const int board_h, Player player, const int fov_x, const int fov_y, vector<Enemy *> &enemies)
{

    message mes;
    mes.text = "";
    mes.priority = 10;

    if (active)
    {
        unsigned int rng = (rand() % 10);

        if (!player.is_dead())
        {
            if (rng + craziness > 9 && turns_doublestep_blocked == 0)
            {
                // two step

                move_relative_to_player(player.get_x(), player.get_y(), 1);
                if (player.get_x() != x || player.get_y() != y) // we don't want to move if we have already caught player
                    move_relative_to_player(player.get_x(), player.get_y(), 1);

                mes.text = "\'\'come closer\'\'";
                mes.priority = 4;

                const int a = 6 - craziness; // just so std::max works
                turns_doublestep_blocked = std::max(1, a);
            }
            else
            {
                if (rng == 0 && turns_shock_blocked == 0)
                {
                    // stand still

                    mes.text = "\'\'ouch\'\'";
                    mes.priority = 4;

                    const int b = 14 - 2 * craziness; // just so std::max works
                    turns_shock_blocked = std::max(1, b);
                }
                else
                {
                    if (rng + 2 * craziness + special_counter > 25)
                    {
                        // teleport back

                        Enemy *en = new FakeGhost(x, y, get_graphic(), "", get_color_pair(), "You just have been killed by an illusion. Great job.");
                        enemies.push_back(en);

                        x += 2 * (player.get_x() - x);
                        y += 2 * (player.get_y() - y);

                        mes.text = "\'\'confusion\'\'";
                        mes.priority = 4;

                        special_counter = 0;
                    }
                    else
                    {
                        // normal follow

                        move_relative_to_player(player.get_x(), player.get_y(), 1);

                        ++special_counter;
                    }

                    // safely decrement turns_shock_blocked
                    turns_shock_blocked = std::max(turns_shock_blocked - 1, 0);
                }

                // safely decrement turns_doublestep_blocked
                turns_doublestep_blocked = std::max(turns_doublestep_blocked - 1, 0);
            }

            //? debug
            //? mes.priority = 1;
            //? mes.text = "rand + crazy: " + std::to_string(a) + "   special_counter: " + std::to_string(special_counter) + "   a>10: " + std::to_string((int)(a>10)) + "   a+c>12: " + std::to_string((int)(a + special_counter > 12));
        }
        else // if player is dead move away
        {
            if (player.get_x() == x && player.get_y() == y)
                move_relative_to_player(player.get_x(), player.get_y(), 1);
            else
                move_relative_to_player(player.get_x(), player.get_y(), -1);
        }

        restrict(x, 0, board_w - 1);
        restrict(y, 0, board_h - 1);

        // turning towards / away from player (depending on whether we're dead or not)
        if (player.get_x() > x)
            display_alt = !player.is_dead();
        else if (player.get_x() < x)
            display_alt = player.is_dead();
        else if (player.get_y() < y)
            display_alt = !player.is_dead();
        else
            display_alt = player.is_dead();
    }

    else
    {
        // if close to player become active
        const bool b1 = (std::abs(player.get_x() - x) < fov_x && std::abs(player.get_y() - y) <= fov_y);
        const bool b2 = (std::abs(player.get_x() - x) <= fov_x && std::abs(player.get_y() - y) < fov_y);
        if (b1 || b2)
        {
            active = true;
            mes.text = "\'\'BOOOO!\'\' Run for your life!";
            mes.priority = 1;
        }
    }

    return mes;
};

string Ghost::get_graphic()
{
    if (active)
    {
        if (display_alt)
            return alt_graphic;
        else
            return graphic;
    }
    return "";
}
