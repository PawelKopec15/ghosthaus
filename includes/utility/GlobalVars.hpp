#pragma once

#include <iostream>
#include <vector>

#include "Vector2.hpp"

struct GlobalVars
{
	// Game engine settings
	uint32_t tickSpeed = 100;

	// Program run flags
	bool debug   = false;
	bool noDelay = false;

	// Variables set in settings
	int colorGrayColor                   = 0;
	std::vector<int> colorGrayAttributes = {};

	// Episode scope variables
	std::string episodePath = "";
	uint32_t inventorySize  = 16;
};