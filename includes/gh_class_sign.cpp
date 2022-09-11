#include <iostream>
#include <ncurses.h>

#include "gh_class_sign.h"

#include "gh_struct_message.h"
#include "gh_class_positionobject.h"

using std::string;

//* Sign -> extends PositionObject
// simple entity with targetmessage() function

Sign::Sign(int x, int y, string sign_text) : PositionObject(x, y)
{
    this->mes.text = sign_text;
    this->mes.priority = 7;
}

message Sign::targetmessage()
{
    return mes;
}
