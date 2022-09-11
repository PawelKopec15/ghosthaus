#pragma once

#include <ncurses.h>
#include <iostream>
#include <vector>
#include <math.h>

#include "gh_simple_functions.h"
#include "gh_struct_message.h"
#include "gh_class_item.h"
#include "gh_constants_and_colors.h"

using std::vector;

//* Inventory class - base
// Provides a functionality of controlling items stored within
// and of drawing inventory to a given window
class Inventory
{
private:
    size_t max_items;
    vector<Item> items;

public:
    Inventory(size_t max_items);
    Inventory(size_t max_items, vector<Item> items);

    bool is_empty();

    void increment_power_of_items();

    // Returns true if inventory has item of given name of false otherwise
    bool has(string item_name);

    // Adds item to inventory
    // returns 0 if successful or 1 if failed (inventory full)
    // (! - is not responsible for display'ing a message
    //  when item is not successfully collected, override Item object's message somehow )
    int add_item(Item item);

    // Removes item from inventory and adds it to ground_items vector with updated position
    // returns a valid message if successful or invalid message if failed to find item
    Item drop_item(int x, int y, WINDOW *win_inventory, const int page, const int selected_item);

    // Finds item by item_name with full power and resets it's power back to 0
    // "afterwards_remove_from_inventory" flag does what you expect
    // returns 0 if successful or 1 if given item with 100% power couldn't be found
    int use_item(string item_name, bool afterwards_remove_from_inventory);

    // Returns all items in vector
    vector<Item> get_all_items();

    // Prints items and inventory information to a window
    // first page is page 0 (!)
    // highlights an item if "highlight_an_item" is true and makes sure highlight is valid
    void draw_inventory(WINDOW *&win_inventory, int &page, bool highlight_an_item, int &selected_item);
};
