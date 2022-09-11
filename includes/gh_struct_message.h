#pragma once

#include <iostream>

// Struct used to represent messages appearing in the window below the main game field
// Contains string text and unsigned char priority
// convention: lower the priority, more important the message
// convention: priority 10 and text "" for invalid message
// convention: text "" for empty message to be ignored
struct message {
    std::string text;
    unsigned char priority;
};