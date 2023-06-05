#include "gh_class_player.hpp"

//* Player -> extends VisibleEntity
// Class used to represent Player entities, contains basically target position

Player::Player(int x, int y, string graphic, char color_pair) : VisibleEntity(x, y, graphic, color_pair)
{
    this->target_x = x;
    this->target_y = y;
    dead = false;
}

// This copy constructor is never used, but I had to have one in my code so here we go
Player::Player(const Player &p)
{
    x = p.x;
    y = p.y;
    target_x = p.target_x;
    target_y = p.target_y;
    dead = p.dead;
    graphic = p.graphic;
    color_pair = p.color_pair;
}

// Changes target_x and target_y based on given movement variables
// ( for example movement_horizontal = 1 increments target_x )
// also makes sure target stays on board
void Player::move_target(int movement_horizontal, int movement_vertical, int board_w, int board_h)
{
    target_x += movement_horizontal;
    target_y += movement_vertical;

    restrict(target_x, 0, board_w - 1);
    restrict(target_y, 0, board_h - 1);
}

// Note: move_target may be more convenient to use
void Player::set_target_position(int target_x, int target_y)
{
    this->target_x = target_x;
    this->target_y = target_y;
}

void Player::set_dead(bool dead)
{
    this->dead = dead;
}

int Player::get_target_x()
{
    return target_x;
}
int Player::get_target_y()
{
    return target_y;
}
bool Player::is_dead()
{
    return dead;
}

void Player::move_to_target_position()
{
    x = target_x;
    y = target_y;
}

message Player::targetmessage()
{
    message mes;
    mes.priority = 7;
    mes.text = "This is impossible to happen, so I can just write anything here";
    return mes;
}
