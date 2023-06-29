#pragma once

enum class ReturnControlValue
{
	CONTINUE           = 0,
	GO_BACK            = 1,
	GO_FORWARD         = 2,
	START_EPISODE      = 3,
	GO_TO_TITLE_SCREEN = 4,
	EXIT_PROGRAM       = 5,
};