#pragma once

#include <iostream>
#include <ncurses.h>

#include "../logic/gh_struct_message.hpp"
#include "gh_class_positionobject.hpp"

using std::string;

//* Sign -> extends PositionObject
// simple entity with targetmessage() function
class Sign : public PositionObject
{
private:
    message mes;

public:
    Sign(int x, int y, string sign_text);

    message targetmessage();
};
