#include <ncurses.h>
#include <unistd.h>  // for usleep()

int main()
{
	// Initialize ncurses
	initscr();
	cbreak();               // Disable line buffering
	noecho();               // Disable echoing of input
	nodelay(stdscr, TRUE);  // Make getch() non-blocking

	// Set a timeout value (in milliseconds)
	int timeout = 10000;  // 10 seconds

	int ch;
	bool timedOut = false;

	while (!timedOut)
	{
		// Check for keyboard input
		ch = getch();

        // clear input buffer
        flushinp();

		if (ch != ERR)
		{
			// Input received
			// Handle the input here
			// ...
			printw("You pressed: %c\n", ch);
			refresh();
		}
		else
		{
			// No input received within the timeout period
			// Perform other tasks or continue the program flow

			// Example: Print a message after timeout
			printw("No input received. Continuing...\n");
			refresh();

			// Wait for a short time before continuing
			usleep(500000);  // Sleep for 0.1 seconds (100,000 microseconds)

			// Example: Exit the program after a certain time
			timeout -= 100;  // Decrease the timeout value
			if (timeout <= 0)
				timedOut = true;
		}
	}

	// Cleanup ncurses
	endwin();

	return 0;
}
