#pragma once

// this enum is used in a lot of places to determine which code should run
enum ActionMode
{
    am_normal,
    am_inventory_drop,
    am_command,
    am_dead,
};