#include "gh_class_positionobject.h"

//* PositionObject - base class
// Base class, has position, getters and setters

PositionObject::PositionObject(int x, int y)
{
    this->x = x;
    this->y = y;
}
PositionObject::PositionObject() {}

int PositionObject::get_x()
{
    return x;
}
int PositionObject::get_y()
{
    return y;
}

void PositionObject::set_position(int x, int y)
{
    this->x = x;
    this->y = y;
}
