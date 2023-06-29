#pragma once

#include "../curseswrapper/Window.hpp"
#include "../utility/Vector2.hpp"
#include "../utility/functions.hpp"
#include "Master.hpp"

class TitleScreenMaster : public Master
{
public:
	explicit TitleScreenMaster(std::shared_ptr<GlobalVars> gvPtr) : Master(gvPtr)
	{
		Curses::Get().Clear();
		Curses::Get().Refresh();
	}

	ReturnControlValue handleInput(int input) override
	{
		if (!hasIntroAlreadyPlayed)
			kill_intro_animation();

		int optionSpacing = 1;
		if (page == Page::MAIN_MENU)
			optionSpacing = 2;

		deluxe_option_cursing(input, highlightedOption, optionSpacing, 0, 8);
		move_arrows();

		return ReturnControlValue::CONTINUE;
	}

	ReturnControlValue handleTick() override
	{
		if (!hasIntroAlreadyPlayed)
			animate_intro();
		else
		{
			animate_win_logo_anim();
			animate_arrows();
		}

		++tickCounter;
		++arrowTickCounter;
		return ReturnControlValue::CONTINUE;
	}

	void terminalSizeChanged() override
	{
		hasIntroAlreadyPlayed = true;
		reposition_windows();
		handle_page_change();
	}

	void terminalSizeInvalid() override
	{ /* empty (for now) */
	}

private:
	enum class Page
	{
		MAIN_MENU       = 0,
		LINK_EASTER_EGG = 1,
		OPTIONS         = 2,
		EPISODE_SELECT  = 3,
	};
	Page page = Page::MAIN_MENU;

	bool hasIntroAlreadyPlayed = false;
	uint32_t highlightedOption = 0;

	Window winLogo      = Window(6, 80);
	Window winLogoAnim  = Window(3, 80);
	Window winMenu      = Window(12, 44);
	Window winExtraText = Window(1, 20);

	Window winRunFlags = Window(2, 20);
	Window winVersion  = Window(2, 20);

	Window winArrowLeft  = Window(1, 3);
	Window winArrowRight = Window(1, 3);

	uint8_t arrowTickCounter = 0;

	const std::array<std::string, 2> arrowLeftAnim  = {"-> ", " ->"};
	const std::array<std::string, 2> arrowRightAnim = {" <-", "<- "};

	const std::wstring introLeft = L" ████   ██  ██   █████    ██████ ████████"
								   L"██  ▀▀  ██  ██  ██   ██  ██         ██   "
								   L"██ ▄▄▄  ██▀▀██  ██   ██   ▀███▄     ██   "
								   L"██  ██  ██  ██  ██   ██       ██    ██   "
								   L" ████   ██  ██   █████   ██████     ██   ";

	const std::wstring introRight = L"██   ██    ▄██▄    ██   ██   ██████"
									L"██   ██   ██▀▀██   ██   ██  ██     "
									L"██▀▀▀██  ██    ██  ██   ██   ▀███▄ "
									L"██   ██  ██▀▀▀▀██  ██   ██       ██"
									L"██   ██  ██    ██   █████   ██████ ";

	const std::wstring logo = L"  ▄████  ██░ ██  ▒█████    ██████ ▄▄▄█████░ ██░  ██  ▄▄▄       █    ██   ██████ "
							  L" ██▒ ▀█▒▓██░ ██▒▒██▒  ██▒▒██    ▒ ▓  ██▒ ▓▒░██░  ██▒▒████▄     ██  ▓██▒▒██    ▒ "
							  L"▒██░▄▄▄░▒██▀▀██░▒██░  ██▒░ ▓██▄   ▒ ▓██░ ▒░▒██▀▀▀██░▒██  ▀█▄  ▓██  ▒██░░ ▓██▄   "
							  L"░▓█  ██▓░▓█ ░██ ▒██   ██░  ▒   ██▒░ ▓██▓ ░ ░▓█  ░██ ░██▄▄▄▄██ ▓▓█  ░██░  ▒   ██▒"
							  L"░▒▓███▀▒░▓█▒░██▓░ ████▓▒░▒██████▒▒  ▒██▒ ░ ░▓█▒░░██▓ ▓█   ▓██▒▒▒█████▓ ▒██████▒▒"
							  L" ░▒   ▒  ▒ ░░▒░▒░ ▒░▒░▒░ ▒ ▒▓▒ ▒ ░  ▒ ░░    ▒ ░▒░▒░▒ ▒▒   ▓▒█░░▒▓▒ ▒ ▒ ▒ ▒▓▒ ▒ ░";

	const std::array<std::wstring, 4> logoAnim = {
		L"  ░   ░  ▒ ░▒░ ░  ░ ▒ ▒░ ░ ░▒  ░ ░          ▒  ░▒░ ░  ▒   ░▒ ░░░▒░ ░ ░ ░ ░▒  ░ ░"
		L"  ░   ░  ░  ░░ ░  ░ ░ ░  ░  ░  ░            ░ ░ ░░ ░  ░   ░  ░ ░░░ ░ ░ ░  ░  ░  "
		L"      ░  ░  ░  ░░   ░ ░        ░    ░       ░   ░  ░      ░      ░           ░  ",

		L"  ░   ░  ▒ ░▒░ ░  ░ ▒ ▒░ ░ ░▒  ░ ░     ░    ▒  ░▒░ ░  ▒   ░▒  ░░▒░ ░ ░ ░ ░▒  ░  "
		L"      ░  ░  ░░ ░  ░ ░ ░  ░  ░  ░            ░ ░ ░░ ░  ░   ░    ░░░   ░ ░  ░  ░ ░"
		L"  ░      ░  ░  ░    ░ ░        ░            ░   ░         ░  ░   ░           ░  ",

		L"  ░   ░  ▒ ░▒░ ░░ ░ ▒ ▒░ ░ ░▒  ░ ░  ░  ░    ▒  ░▒░ ░  ▒   ░▒  ░░▒░ ░ ░ ░ ░▒  ░  "
		L"         ░  ░░ ░  ░ ░ ░  ░  ░  ░       ░    ░ ░ ░░    ░   ░    ░░░ ░ ░ ░  ░  ░  "
		L"         ░  ░  ░    ░ ░        ░            ░   ░         ░      ░           ░ ░",

		L"  ░   ░  ▒ ░▒░ ░  ░ ▒ ▒░ ░ ░▒  ░ ░     ░    ▒  ░▒░ ░  ▒   ░▒ ░░░▒░ ░ ░ ░ ░▒  ░  "
		L"      ░  ░  ░░ ░░ ░ ░ ░  ░  ░  ░    ░       ░ ░ ░░ ░  ░   ░    ░░░ ░ ░ ░  ░  ░  "
		L"         ░  ░  ░    ░ ░        ░            ░   ░         ░      ░ ░         ░  "};

	void handle_page_change()
	{
		highlightedOption = 0;

		switch (page)
		{
			default:  // case Page::MAIN_MENU:
				winMenu.drawBox('+', '+');

				winMenu.printAt("NEW GAME", 2, 18);
				winMenu.printAt("CONTINUE (WIP)", 4, 18);
				winMenu.printAt("SETTINGS (INDEV)", 6, 18);
				winMenu.printAt("QUIT", 8, 20);
				break;
		}

		winMenu.refresh();
	}

	int calculate_logo_y() const { return (Curses::Get().GetTerminalCenterY() - winLogo.getHeight()) / 2 - 2; }

	void initialize_win_logo()
	{
		winLogo.correctAfterTerminalResizing();
		winLogo.setScrollLock(false);

		for (uint32_t i = 0; i < logo.length(); ++i)
		{
			if (logo[i] == L'░')
				winLogo.setColor(COLOR_RED, COLOR_BLACK);
			else if (logo[i] == L'▒' && i % 3 == 0)
				winLogo.setColor(COLOR_RED, COLOR_BLACK);
			else
				winLogo.setColor(COLOR_WHITE, COLOR_BLACK);

			winLogo.print(logo[i]);
		}
	}

	void reposition_win_logo()
	{
		winLogo.move(calculate_logo_y(), winLogo.getCenteredPositionX());
		winLogo.refresh();
	}

	void move_arrows()
	{
		arrowTickCounter = 0;

		winArrowLeft.erase();
		winArrowRight.erase();

		winArrowLeft.refresh();
		winArrowRight.refresh();

		winArrowLeft.moveToY(winMenu.getGlobalPositionY() + highlightedOption + 2);
		winArrowRight.moveToY(winArrowLeft.getGlobalPositionY());
	}

	void initialize_windows()
	{
		initialize_win_logo();
		winLogoAnim.correctAfterTerminalResizing();
		winLogoAnim.setScrollLock(false);

		winMenu.correctAfterTerminalResizing();

		winRunFlags.correctAfterTerminalResizing();
		if (globalVarsPtr->debug)
			winRunFlags.printAt("Debug enabled.", 0, 1);
		if (globalVarsPtr->noDelay)
			winRunFlags.printAt("No delay enabled.", 1, 1);

		winVersion.correctAfterTerminalResizing();
		winVersion.printAt("Ghosthaus ver 1.2", 0, 2);
		winVersion.printAt(" Paweł Kopeć 2023", 1, 2);

		winExtraText.correctAfterTerminalResizing();
		winExtraText.setColor(COLOR_RED, COLOR_BLACK);
		winExtraText.print("-- SELECT EPISODE --");

		winArrowLeft.correctAfterTerminalResizing();
		winArrowLeft.setColor(COLOR_RED, COLOR_BLACK);
		winArrowLeft.print(arrowLeftAnim[0]);

		winArrowRight.correctAfterTerminalResizing();
		winArrowRight.setColor(COLOR_RED, COLOR_BLACK);
		winArrowRight.print(arrowRightAnim[0]);
	}

	void animate_win_logo_anim()
	{
		if (tickCounter % 2 == 0)
		{
			const std::wstring frame = logoAnim[(tickCounter / 2) % logoAnim.size()];
			winLogoAnim.moveCursor(0, 0);

			for (uint32_t i = 0; i < frame.size(); ++i)
			{
				if (frame[i] == L'░')
					winLogoAnim.setColor(COLOR_RED, COLOR_BLACK);
				else if (frame[i] == L'▒' && i % 3 == 0)
					winLogoAnim.setColor(COLOR_RED, COLOR_BLACK);
				else
					winLogoAnim.setColor(COLOR_WHITE, COLOR_BLACK);

				winLogoAnim.print(frame[i]);
			}
			winLogoAnim.refresh();
		}
	}

	void animate_arrows()
	{
		if (arrowTickCounter % 7 == 0)
		{
			const int i = arrowTickCounter % 2;

			winArrowLeft.printAt(arrowLeftAnim[i], 0, 0);
			winArrowRight.printAt(arrowRightAnim[i], 0, 0);

			winArrowLeft.refresh();
			winArrowRight.refresh();
		}
	}

	void reposition_windows()
	{
		Curses::Get().Clear();
		Curses::Get().Refresh();

		initialize_windows();

		reposition_win_logo();
		winLogoAnim.move(winLogo.getGlobalPositionY() + winLogo.getHeight(), winLogo.getCenteredPositionX());

		winMenu.move(Curses::Get().GetTerminalCenterY() - 1, winMenu.getCenteredPositionX());

		winRunFlags.move(Curses::Get().GetTerminalHeight() - 2, 0);
		winVersion.move(Curses::Get().GetTerminalHeight() - 2,
						Curses::Get().GetTerminalWidth() - winRunFlags.getWidth());

		winExtraText.move(winMenu.getGlobalPositionY() - 1, winExtraText.getCenteredPositionX());

		winArrowLeft.move(winMenu.getGlobalPositionY() + highlightedOption + 2, winMenu.getCenteredPositionX() + 4);
		winArrowRight.move(winArrowLeft.getGlobalPositionY(),
						   winMenu.getCenteredPositionX() + winMenu.getWidth() - 4 - winArrowRight.getWidth());

		winRunFlags.refresh();
		winVersion.refresh();
	}

	void kill_intro_animation()
	{
		Curses::Get().Clear();
		Curses::Get().EndWin();
		Curses::Get().ReInit();
		terminalSizeChanged();
	}

	void animate_intro()
	{
		const uint32_t half_screen = Curses::Get().GetTerminalWidth() / 2;
		const uint32_t right_shift = 2;

		const uint32_t text_height = 5;
		const uint32_t ghost_width = (uint32_t)introLeft.size() / text_height;
		const uint32_t haus_width  = (uint32_t)introRight.size() / text_height;

		const uint32_t num_spaces_left  = std::max((int)tickCounter - (int)ghost_width, 0);
		const uint32_t num_spaces_right = std::max((int)tickCounter - (int)haus_width, 0);

		if (tickCounter <= half_screen + right_shift)
		{
			Curses::Get().MoveCursor(calculate_logo_y(), 0);

			for (uint32_t i = 0; i < text_height; ++i)
			{
				Curses::Get().MoveCursor(calculate_logo_y() + i, 0);

				// Printing spaces on the left
				for (uint32_t j = num_spaces_left; j > 0; --j)
					Curses::Get().Print(' ');

				// Printing a row of "GHOST" text
				for (uint32_t j = tickCounter - num_spaces_left; j > 0; --j)
					Curses::Get().Print(introLeft[((i + 1) * ghost_width) - j]);
			}

			if (tickCounter < half_screen - right_shift)
			{
				for (uint32_t i = 0; i < text_height; ++i)
				{
					Curses::Get().MoveCursor(calculate_logo_y() + i, Curses::Get().GetTerminalWidth() - tickCounter);

					// Printing a row of "HAUS" text
					for (uint32_t j = 0; j < tickCounter - num_spaces_right; ++j)
						Curses::Get().Print(introRight[(i * haus_width) + j]);

					// Printing spaces on the right (mainly to override whatever was written before)
					for (uint32_t j = num_spaces_right; j > 0; --j)
						Curses::Get().Print(' ');
				}
			}
		}
		else if (tickCounter < half_screen + 5)
		{
			/* wait */
		}
		else if (tickCounter < half_screen + 8)
		{
			Curses::Get().Clear();
			Curses::Get().Refresh();
		}
		else if (tickCounter < half_screen + 10)
		{
			initialize_win_logo();
			reposition_win_logo();
		}
		else if (tickCounter < half_screen + 14)
		{
			winLogo.clear();
			winLogo.refresh();
		}
		else if (tickCounter < half_screen + 20)
		{
			initialize_win_logo();
			winLogo.refresh();
		}
		else
		{
			kill_intro_animation();
			tickCounter = 0;
		}

		Curses::Get().Refresh();
	}
};
