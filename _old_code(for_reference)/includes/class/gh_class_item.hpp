#pragma once

#include <iostream>

#include "gh_class_visibleentity.hpp"

using std::string;

//* Item -> extends VisibleEntity
// Constains variables and functionality for both items lying on the ground
// and items in the inventory
class Item : public VisibleEntity
{

private:
    string name;
    int power;
    int max_power;
    string message_when_collected;
    int score;

public:
    Item(int x, int y, string graphic, unsigned char color_pair, string name, string message_when_collected, int score);
    Item(int x, int y, string graphic, unsigned char color_pair, string name, string message_when_collected, int max_power, int score);

    string get_name();

    void set_max_power(int max_power);

    int get_max_power();

    double get_power_fraction();

    bool is_ready_to_use();

    // If successful sets power to 0 and returns 0
    // If not returns 1
    int use();

    void increment_power();

    int get_score();

    message targetmessage();
};