#### Ghosthaus ver 1.0
#### Paweł Kopeć
#### 11 September 2022

--------------------------------------------------------------------------------

# Changelog

## [1.0] - 2022-09-11
### Added
- Version info on title screen (finally)
- Finished the first 4 levels of spooky house adventure episode
- New command - :invincible (sets to true or resets to false)
  You can't be killed by an enemy if invincible is set to true
- Now you know how much a gemstone is worth when you collect it
- Many bug fixes
- You can interact with tables to get a boring message now
- Finished README2, now it contains a full episode and level building guide
- Improved README (as always)
- Switchable floors have a new look

--------------------------------------------------------------------------------
note: these versions are milestones and not actual previous commits, the first
commit is version 1.0

## [beta 2] - 2022-09-06
### Added
- Stopwatch item, that freezes both enemies and doors for some time
- Green gemstone, red gemstone
- Much nicer level transitions
- You can specify the message that displays after beating the level in level
  data (probably since beta 1 but forgot to mention it)
- Absolutely wonderful dying animation
- Level restart after death, with a message and score penalty
- You can customize the above mentioned message in level data
- New ghost behavior if player is dead
- There is now a message after completing an episode, which displays your score
  and takes you back to title screen
- Also you can exit to title screen by pressing F7
- Switchable walls and floors are green now, so you can recognize them
- New command - :die (does what you expect)
- Changes in level layouts
- Minor aesthetic changes

## [beta 1.1] - 2022-09-03
### Changes
- Attempting to fix one very specific bug in render

## [beta 1] - 2022-09-03
### Added
- New, improved level data system, that is more user friendly and more foolproof
- Related to above point, improved tile encoding system, which now comes with
  a nice instruction table (see README2 in episodes folder)
- You can customize many things in level data now, such as ghost craziness level
  and guard enemies' behavior, all four of them independently (will be described
  in README2 in episodes folder)
- Level transitions, with score counting
- Episode intro text
- Fake wall tile
- Red door, red key (work same as green counterparts)
- Blue gemstone (useless, but valuable item)
- New, improved ghost AI
- Ghost looks better when it is in the wall
- Now you get instantly killed when trying to walk into an enemy
- You can "skip your turn" (stand still while everything else moves) by pressing
  space (debug only)
- Minor aesthetic changes
- Changes in level layouts
- Began work on 2.txt in /spooky house adventure/ (very unfinished)

## [indev 3] - 2022-08-28
### Added
- Ghost enemy (finally)
- New items (camera, skeleton key, super flashlight)
- Working camera by pressing C, it creates a flash that stuns all enemies for
  a couple turns
- Guard enemies
- Switchable walls, switchable floor
- New switch object, that turns switchable walls to switchable floors and
  vice versa when targeted by player
- More debug information
- There are more possible flashlight levels than just two now
- Changes to tiles and maps
- Changes in level layouts
- Minor aesthetic changes

- Commands
  Reworked the vim easter egg into working commands, if debug mode is enabled
  you can press : to start typing a command:
  :give <item> (green key, flashlight, camera, super flashlight, skeleton key)
  :light up, :light down
  :wq, :q!, :quit, :exit

### Important improvements
- Fixed an annoying problem with input stacking uncontrollably


## [indev 2] - 2022-08-16
### Added
- Dropping items witch D ( select an item to drop with arrow keys, then drop
  highlighted item with Enter or cancel with D ( sadly I can't use Esc ) )
- Completely new episode system with .ep files
- Completely new episode select screen
- New episode "GAME TESTER'S DELIGHT"
- Title screen SETTINGS page with option to select how to represent gray color
- Minor changes to tiles and maps
- Minor aesthetic changes (red arrows on title screen ect)
- Changes in level layouts
- Small vim easter egg

## [indev 1] - 2022-08-12
### Added
- 100% of things that are currently here

--------------------------------------------------------------------------------

-> Compiling the game

The game must be compiled with -lncursesw linker flag, which requires ncurses
to be installed as it is not a default library
Also the c++ version has to be set to c++ 17 or newer

Makefile:
make spooky - to compile the game
make clean  - to erase the compiled game

Copypaste:
g++ main.cpp includes/*.cpp -o ghosthaus.x -Wall -lncursesw -std=c++17

Once compiled, an executable ghostaus.x will be ready to be run.

--------------------------------------------------------------------------------
-> Terminal info:

Terminal of at least 80 columns by 24 rows is required.
(preferably about 120 columns by 34 rows for best experience)
Colored terminal is recommended. It will run on a non colored one, but the
experience will be far from desirable.
If game fails to set font to unicode it won't run.
Resizing terminal window is not a good idea, although the game corrects itself
when exiting the title screen to start the game, so you can resize the terminal
just before pressing NEW GAME, selecting an episode or going back to title
screen.

Game has been developed in WSL, tested in WSL and Fedora.
Should any compatibility issues surface let me know.

The game should be run from the folder it's in.

Recommended fonts:
On WSL- Consolas and (maybe) Courier New are the best fonts for everything to
display properly.
Consolas looks very nice when bold, but not Courier New.

This game uses bold version of color black to represent color gray, on some
terminals it looks fine but on others it might be better to change how color
gray looks in game setting on the title screen.

--------------------------------------------------------------------------------
-> Run flags:

There are optional run flags the game recognizes:

debug (displays some info on top of the screen and grands access to some
       handy debug features):
  -d or -debug
no delay (cancels a slight delay that happens every turn before input):
  -nd or -nodelay

--------------------------------------------------------------------------------
-> Gameplay info:

The game is about walking through various levels, solving simple puzzles,
collecting items to progress further or to gain score and reaching the exit
on each level of an episode.
Avoid mighty guards and the spooky ghost or suffer the consequences.

--------------------------------------------------------------------------------
-> Game controls:

Arrow keys  movement, interacting with environment, selecting things
Enter       confirming things
Page Up     inventory previous page
Page Down   inventory next page
D           dropping items
C           using the camera's flash
S           using the stopwatch
F7          exit to title screen
F10         exit the game
Space       skip turn (when debug is enabled)
:           enter command mode (when debug is enabled)

--------------------------------------------------------------------------------
-> Debug commands:

:give <item>
  where <item> can be:
  green key, red key, flashlight, camera, super flashlight, skeleton key
  stopwatch, blue gem, red gem, green gem
:light up, :light down
  (manipulates the field of view)
:wq, :q!, :quit, :exit
  (exits the game)
:die
  (does what you expect)
:invincible
  (toggles invincibility, you can't be killed by an enemy if invincible)
