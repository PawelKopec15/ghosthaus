#include "gh_singleton_inventory.hpp"

using std::vector;

//* Inventory class - base
// Provides a functionality of controlling items stored within
// and of drawing inventory to a given window

void Inventory::SetPage(int val) { page = val; }
void Inventory::IncrementPage() { page++; }
void Inventory::DecrementPage() { page--; }
int Inventory::GetPage() const { return page; };

void Inventory::SetSelectedItem(int val) { selected_item = val; }
void Inventory::IncrementSelectedItem() { selected_item++; }
void Inventory::DecrementSelectedItem() { selected_item--; }
int Inventory::GetSelectedItem() const { return selected_item; };

void Inventory::SetMaxItems(int val) { max_items = val; }

void Inventory::SetItems(vector<Item> &_items) { items = _items; }

bool Inventory::IsEmpty() const { return items.empty(); }

void Inventory::IncrementPowerOfItems()
{
	for (auto it = begin(items); it != end(items); ++it)
	{
		it->increment_power();
	}
}

// Returns true if inventory has item of given name of false otherwise
bool Inventory::Has(const string &item_name)
{
	for (auto it = begin(items); it != end(items); ++it)
	{
		if (it->get_name().compare(item_name) == 0)
			return true;
	}
	return false;
}

// Adds item to inventory
// returns 0 if successful or 1 if failed (inventory full)
// (! - is not responsible for display'ing a message
//  when item is not successfully collected, override Item object's message somehow )
int Inventory::AddItem(Item &item)
{
	if (items.size() < max_items)
	{
		items.push_back(item);
		return 0;
	}
	return 1;
}

// Removes item from inventory and adds it to ground_items vector with updated position
// returns a valid message if successful or invalid message if failed to find item
Item Inventory::DropItem(int x, int y)
{
	const int item_index = selected_item + page * (getmaxy(Camera::Get().GetWinInventory()) - 5);

	Item item_to_drop = items.at(item_index);
	items.erase(items.begin() + item_index);

	item_to_drop.set_position(x, y);
	return item_to_drop;
}

// Finds item by item_name with full power and resets it's power back to 0
// "afterwards_remove_from_inventory" flag does what you expect
// returns 0 if successful or 1 if given item with 100% power couldn't be found
int Inventory::UseItem(const string &item_name, bool afterwards_remove_from_inventory)
{
	for (auto it = begin(items); it != end(items);)
	{
		if (item_name == it->get_name() && it->is_ready_to_use())
		{
			if (it->use() == 0)
			{
				if (afterwards_remove_from_inventory)
					it = items.erase(it);

				return 0;
			}
		}
		else
			++it;
	}
	return 1;
}

// Returns all items in vector
vector<Item> Inventory::GetAllItems() const { return items; }

// Prints items and inventory information to a window
// first page is page 0 (!)
// highlights an item if "highlight_an_item" is true and makes sure highlight is valid
void Inventory::DrawInventory(bool highlight_an_item)
{
	WINDOW *&win_inventory = Camera::Get().GetWinInventory();

	const int win_height  = getmaxy(win_inventory);
	const int page_height = win_height - 5;

	const int page_count = get_page_count(items.size(), page_height);
	restrict(page, 0, page_count - 1);

	// var a exists only so std::min doesn't complain
	const int a                    = items.size() - page * page_height;
	const int items_names_to_print = std::min(page_height, a);

	const int v_offset = 3;

	// highlight
	if (highlight_an_item)
		restrict_wrap_around(selected_item, 0, (items_names_to_print - 1));

	wbkgd(win_inventory, COLOR_PAIR(0));
	werase(win_inventory);
	box(win_inventory, 0, 0);

	mvwprintw(win_inventory, 1, 2, "---- ITEMS ----");

	if (page_count > 1)
		mvwprintw(win_inventory, 2, 6, ("page  " + std::to_string(page)).c_str());

	for (int i = 0; i < items_names_to_print; ++i)
	{
		const int item_id = i + page * page_height;
		if (i == selected_item && highlight_an_item)
			wattron(win_inventory, A_REVERSE);

		mvwprintw(win_inventory, v_offset + i, 2, items.at(item_id).get_name().c_str());

		wattroff(win_inventory, A_REVERSE);
		// printing power percentage
		const int item_power_percentage = std::ceil(items.at(item_id).get_power_fraction() * 100.0);
		if (items.at(item_id).get_max_power() != 1)
		{
			string toprint;
			if (item_power_percentage == 100)
				toprint = "- √";
			else
				toprint = ("-" + std::to_string(item_power_percentage) + "%%");
			mvwprintw(win_inventory, v_offset + i, getmaxx(win_inventory) - 6, toprint.c_str());
		}
	}

	if (page_count - 1 > page)
		mvwprintw(win_inventory, win_height - 1, INVENTORY_WINDOW_WIDTH - 7, "PgDn->");
	if (page > 0)
		mvwprintw(win_inventory, win_height - 1, 1, "<-PgUp");

	wrefresh(win_inventory);
}
