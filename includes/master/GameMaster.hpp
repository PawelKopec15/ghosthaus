#pragma once

#include <cstring>
#include <memory>
#include <vector>

#include "../curseswrapper/Curses.hpp"
#include "../singleton/Debugger.hpp"
#include "../utility/GlobalVars.hpp"
#include "../utility/ReturnControlValue.hpp"
#include "Master.hpp"
#include "TitleScreenMaster.hpp"

class GameMaster
{
public:
	static GameMaster &Get()
	{
		static GameMaster INSTANCE;
		return INSTANCE;
	}
	GameMaster(GameMaster &&)      = delete;
	GameMaster(const GameMaster &) = delete;
	GameMaster &operator=(GameMaster &&) = delete;
	GameMaster &operator=(const GameMaster &) = delete;

	void InitProgram(int argc, char const *argv[])
	{
		Curses::Get().Init();

		find_flags(argc, argv);
		if (globalVariables->debug)
			Debugger::Get().EnableDebug();

		change_scene(GameScene::TITLE_SCREEN);

		ValidateTerminalSize();

		Debugger::Get().Log("GameMaster: Program initialization done.");
	}

	ReturnControlValue Run()
	{
		while (true)
		{
			if (terminalSizeValid && Loop() == ReturnControlValue::EXIT_PROGRAM)
				return ReturnControlValue::EXIT_PROGRAM;
		}
	}

	ReturnControlValue Loop()
	{
		//  ||--------------------------------------------------------------------------------||
		//  ||                                    main loop                                   ||
		//  ||--------------------------------------------------------------------------------||
		int input = Curses::Get().GetChFlushInp();

		auto inputRetConVal   = ReturnControlValue::CONTINUE;
		auto tickRetConVal    = ReturnControlValue::CONTINUE;
		auto checkedRetconVal = ReturnControlValue::CONTINUE;

		if (input != ERR)
			inputRetConVal = currentMaster->handleInput(input);
		else
		{
			tickRetConVal = currentMaster->handleTick();
			sleep_millis(globalVariables->tickSpeed);  // if no key has been pressed introduce a delay
		}

		if (inputRetConVal != ReturnControlValue::CONTINUE)
			checkedRetconVal = inputRetConVal;
		else if (tickRetConVal != ReturnControlValue::CONTINUE)
			checkedRetconVal = tickRetConVal;

		switch (checkedRetconVal)
		{
			default:
				break;

			case ReturnControlValue::EXIT_PROGRAM:
				return ReturnControlValue::EXIT_PROGRAM;

			case ReturnControlValue::GO_TO_TITLE_SCREEN:
				change_scene(GameScene::TITLE_SCREEN);
				break;

			case ReturnControlValue::START_EPISODE:
				change_scene(GameScene::MAIN_GAME_ACTION);
				break;
		}
		return ReturnControlValue::CONTINUE;
	}

	void ValidateTerminalSize()
	{
		Vector2<int> terminalSize = Curses::Get().GetTerminalSize();
		if (terminalSize.x < 80 || terminalSize.y < 24)
		{
			terminalSizeValid = false;
			Curses::Get().Clear();

			Curses::Get().Print("Terminal size of at least 80 columns by 24 rows is required.\n");
			Curses::Get().Print("( Current size: " + std::to_string(terminalSize.x) + " columns by " +
								std::to_string(terminalSize.y) + " rows. )\n");
			Curses::Get().Print("Resize the terminal to continue.");

			Curses::Get().Refresh();
		}
		else
			terminalSizeValid = true;
	}

	void TerminalSizeChanged()
	{
		Debugger::Get().Log("GameMaster: Terminal size changed");
		Curses::Get().Clear();
		Curses::Get().EndWin();
		Curses::Get().ReInit();

		ValidateTerminalSize();

		if (terminalSizeValid)
			currentMaster->terminalSizeChanged();
		else
			currentMaster->terminalSizeInvalid();
	}

private:
	enum class GameScene
	{
		TITLE_SCREEN     = 0,
		MAIN_GAME_ACTION = 1,
	};

	bool terminalSizeValid = true;

	GameMaster() = default;

	std::shared_ptr<GlobalVars> globalVariables = std::make_shared<GlobalVars>();

	GameScene currentGameScene;
	std::unique_ptr<Master> currentMaster;

	void find_flags(int argc, char const *argv[]) const
	{
		for (int i = 1; i < argc; ++i)
		{
			if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "-debug"))
				globalVariables->debug = true;
			if (!strcmp(argv[i], "-nd") || !strcmp(argv[i], "-nodelay"))
				globalVariables->noDelay = true;
		}
	}

	void change_scene(const GameScene &newScene)
	{
		currentGameScene = newScene;

		switch (newScene)
		{
			default:  // case GameScene::TITLE_SCREEN:
				currentMaster = std::make_unique<TitleScreenMaster>(globalVariables);
				Debugger::Get().Log("GameMaster:: Changed scene to TITLE_SCREEN.");
				break;

			case GameScene::MAIN_GAME_ACTION:
				// TODO: levelMaster or something
				Debugger::Get().Log("GameMaster:: Changed scene to MAIN_GAME_ACTION.");
				break;
		}
	}

	void sleep_millis(unsigned long milliseconds)
	{
		struct timespec req;
		req.tv_sec  = milliseconds / 1000;
		req.tv_nsec = (milliseconds % 1000) * 1000000;
		nanosleep(&req, NULL);
	}
};
