#include <iostream>
#include <math.h>

#include "gh_class_item.h"

#include "gh_class_visibleentity.h"

using std::min;
using std::string;

//* Item -> extends VisibleEntity
// Constains variables and functionality for both items lying on the ground
// and items in the inventory

Item::Item(int x, int y, string graphic, unsigned char color_pair, string name, string message_when_collected, int score) : VisibleEntity(x, y, graphic, color_pair)
{
    this->name = name;
    this->message_when_collected = message_when_collected;

    power = 1;
    max_power = 1;

    this->score = score;
}
Item::Item(int x, int y, string graphic, unsigned char color_pair, string name, string message_when_collected, int max_power, int score) : VisibleEntity(x, y, graphic, color_pair)
{
    this->name = name;
    this->message_when_collected = message_when_collected;

    this->max_power = max_power;
    power = max_power;

    this->score = score;
}

string Item::get_name()
{
    return name;
}

void Item::set_max_power(int max_power)
{
    this->max_power = max_power;
    power = max_power;
}

int Item::get_max_power()
{
    return max_power;
}

double Item::get_power_fraction()
{
    return ((double)power / (double)max_power);
}
bool Item::is_ready_to_use()
{
    return (power == max_power);
}

// If successful sets power to 0 and returns 0
// If not returns 1
int Item::use()
{
    if (is_ready_to_use())
    {
        power = 0;
        return 0;
    }
    return 1;
}
void Item::increment_power()
{
    power = min(power + 1, max_power);
}

int Item::get_score()
{
    return score;
}

message Item::targetmessage()
{
    message mes;
    mes.text = message_when_collected;
    mes.priority = 2;
    return mes;
}
