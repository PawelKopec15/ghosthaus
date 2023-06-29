#include "gh_class_visibleentity.hpp"

using std::string;

//* VisibleEntity -> extends PositionObject
// Abstract class containing render information: string graphic and unsigned char color_pair
// Also virtual message targetmessage() = 0 so this code contains a pure virtual function somewhere

VisibleEntity::VisibleEntity(int x, int y, string graphic, char color_pair) : PositionObject(x, y)
{
    this->graphic = graphic;
    this->color_pair = color_pair;
}
VisibleEntity::VisibleEntity() {}

string VisibleEntity::get_graphic()
{
    return graphic;
}
unsigned char VisibleEntity::get_color_pair()
{
    return color_pair;
}

void VisibleEntity::set_graphic(string graphic)
{
    this->graphic = graphic;
}
void VisibleEntity::set_color_pair(unsigned char color_pair)
{
    this->color_pair = color_pair;
}
