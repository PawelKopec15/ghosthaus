#pragma once

#include <iostream>

#include "gh_constants_and_colors.h"

//* Tile - base class
// Constains tile info, such as it's graphics, message number and whether you can pass through it or not
class Tile
{
private:
    bool passthrough;
    unsigned char graphics_id;
    unsigned char color_pair;
    unsigned char message_num;

public:
    Tile();
    Tile(bool passthrough, unsigned char graphics_id, unsigned char color_pair, unsigned char message_num);
    Tile(bool passthrough, unsigned char graphics_id, unsigned char color_pair);

    bool is_passable();

    unsigned char get_graphics_id();

    unsigned char get_color_pair();

    unsigned char get_message_num();

    void set_color_pair(unsigned char color_pair);

    bool operator==(Tile t);

    // Unused, but sounds cool so who knows, maybe one day...
    void operator+(int val);
};