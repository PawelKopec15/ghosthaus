#pragma once

//* PositionObject - base class
// Base class, has position, getters and setters
class PositionObject
{

protected:
    int x=0, y=0;

public:
    PositionObject(int x, int y);
    PositionObject();

    int get_x();
    
    int get_y();

    void set_position(int x, int y);
};
