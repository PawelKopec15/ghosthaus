#pragma once

#include <iostream>
#include <ncurses.h>

#include "gh_struct_message.h"
#include "gh_class_positionobject.h"

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
