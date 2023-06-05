#pragma once

#include <ncurses.h>

#include <cmath>
#include <iostream>
#include <vector>

#include "../class/gh_class_item.hpp"
#include "../logic/gh_constants_and_colors.hpp"
#include "../logic/gh_simple_functions.hpp"
#include "../logic/gh_struct_message.hpp"
#include "gh_singleton_camera.hpp"


using std::vector;

//* Inventory class - base
// Provides a functionality of controlling items stored within
// and of drawing inventory to a given window
class Inventory
{
public:
	Inventory(const Inventory&) = delete;

	static Inventory& Get()
	{
		static Inventory INSTANCE;
		return INSTANCE;
	}

	void SetPage(int val);
	void IncrementPage();
	void DecrementPage();
	int GetPage() const;

	void SetSelectedItem(int val);
	void IncrementSelectedItem();
	void DecrementSelectedItem();
	int GetSelectedItem() const;

	void SetMaxItems(int val);

	void SetItems(vector<Item>& _items);

	bool IsEmpty() const;

	void IncrementPowerOfItems();

	// Returns true if inventory has item of given name of false otherwise
	bool Has(const string& item_name);

	// Adds item to inventory
	// returns 0 if successful or 1 if failed (inventory full)
	// (! - is not responsible for display'ing a message
	//  when item is not successfully collected, override Item object's message somehow )
	int AddItem(Item& item);

	// Removes item from inventory and adds it to ground_items vector with updated position
	// returns a valid message if successful or invalid message if failed to find item
	Item DropItem(int x, int y);

	// Finds item by item_name with full power and resets it's power back to 0
	// "afterwards_remove_from_inventory" flag does what you expect
	// returns 0 if successful or 1 if given item with 100% power couldn't be found
	int UseItem(const string& item_name, bool afterwards_remove_from_inventory);

	// Returns all items in vector
	vector<Item> GetAllItems() const;

	// Prints items and inventory information to a window
	// first page is page 0 (!)
	// highlights an item if "highlight_an_item" is true and makes sure highlight is valid
	void DrawInventory(bool highlight_an_item);

private:
	Inventory() = default;

	int page;
	int selected_item;

	size_t max_items;
	vector<Item> items;
};
