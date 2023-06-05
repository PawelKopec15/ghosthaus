#include "gh_class_sign.hpp"

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
