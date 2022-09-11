#pragma once

#include <iostream>

#include "gh_struct_message.h"
#include "gh_class_positionobject.h"

using std::string;

//* VisibleEntity -> extends PositionObject
// Abstract class containing render information: string graphic and unsigned char color_pair
// Also virtual message targetmessage() = 0 so this code contains a pure virtual function somewhere
class VisibleEntity : public PositionObject
{

protected:
    string graphic = "";
    unsigned char color_pair = 0;

public:
    VisibleEntity(int x, int y, string graphic, char color_pair);
    VisibleEntity();

    virtual string get_graphic();

    unsigned char get_color_pair();

    void set_graphic(string graphic);

    void set_color_pair(unsigned char color_pair);

    virtual message targetmessage() = 0;
};