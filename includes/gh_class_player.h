#pragma once

#include "gh_class_visibleentity.h"
#include "gh_simple_functions.h"

//* Player -> extends VisibleEntity
// Class used to represent Player entities, contains basically target position
class Player : public VisibleEntity
{
private:
    int target_x, target_y;
    bool dead;

public:
    Player(int x, int y, string graphic, char color_pair);

    // This copy constructor is never used, but I had to have one in my code so here we go
    Player(const Player &p);

    // Changes target_x and target_y based on given movement variables
    // ( for example movement_horizontal = 1 increments target_x )
    // also makes sure target stays on board
    void move_target(int movement_horizontal, int movement_vertical, int board_w, int board_h);

    // Note: move_target may be more convenient to use
    void set_target_position(int target_x, int target_y);

    void set_dead(bool dead);

    int get_target_x();

    int get_target_y();

    bool is_dead();

    void move_to_target_position();

    message targetmessage();
};
