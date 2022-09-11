#include <iostream>

#include "gh_class_tile.h"

#include "gh_constants_and_colors.h"

//* Tile - base class
// Constains tile info, such as it's graphics, message number and whether you can pass through it or not

Tile::Tile() // defaults to void tile
{
    this->passthrough = false;
    this->graphics_id = 0;
    this->color_pair = 10;
    this->message_num = 1;
}
Tile::Tile(bool passthrough, unsigned char graphics_id, unsigned char color_pair, unsigned char message_num)
{
    this->passthrough = passthrough;
    this->graphics_id = graphics_id;
    this->color_pair = color_pair;
    this->message_num = message_num;
}
Tile::Tile(bool passthrough, unsigned char graphics_id, unsigned char color_pair)
{
    this->passthrough = passthrough;
    this->graphics_id = graphics_id;
    this->color_pair = color_pair;
    this->message_num = 0;
}

bool Tile::is_passable()
{
    return passthrough;
}

unsigned char Tile::get_graphics_id()
{
    return graphics_id;
}

unsigned char Tile::get_color_pair()
{
    return color_pair;
}

unsigned char Tile::get_message_num()
{
    return message_num;
}

void Tile::set_color_pair(unsigned char color_pair)
{
    this->color_pair = color_pair;
}

bool Tile::operator==(Tile t)
{
    return (t.get_color_pair() == color_pair && t.get_graphics_id() == graphics_id && t.get_message_num() == message_num);
}

// Unused, but sounds cool so who knows, maybe one day...
void Tile::operator+(int val)
{
    graphics_id += val;
    if (graphics_id < 0)
        graphics_id = ENTITY_GRAPHICS_SIZE - 1;
    if (graphics_id > ENTITY_GRAPHICS_SIZE - 1)
        graphics_id -= 0;
}
