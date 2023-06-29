#include <csignal>

#include "../includes/master/GameMaster.hpp"
#include "../includes/utility/ReturnControlValue.hpp"

void resizeHandler([[maybe_unused]] int sig) { GameMaster::Get().TerminalSizeChanged(); }

int main(int argc, char const* argv[])
{
	GameMaster::Get().InitProgram(argc, argv);

	signal(SIGWINCH, resizeHandler);

	while (true)
	{
		if (GameMaster::Get().Run() == ReturnControlValue::EXIT_PROGRAM)
			break;
	}

	return 0;
}