# Ghosthaus ver 1.1

<br>

![Screenshot of the title screen](/media/image1.png)

##### My goal and self imposed challenge of this project was to create the best looking video game I realistically could using ncurses.h library.<br>This was primarily developed as a project for my university C++ course.<br><br>

![Short footage of the beginning of the game](/media/movie1.gif)

![Screenshot of what You see when You're about to lose](/media/image3.png)



----------------------------------------------------------------------------
## Gameplay info:

<br>
In this simple game you travel through various levels, solving simple puzzles, collecting items to progress further or to gain score and try to reach the exit on each level of an episode.
<br>

![Random screenshot of the game](/media/image2.png)

Avoid mighty guards and the spooky ghost or suffer the consequences.
<br>

![Footage of ghost encounter](/media/movie2.gif)

----------------------------------------------------------------------------

----------------------------------------------------------------------------

## Running the precompiled game

<br>
If You don't want to compile the game, the precompiled ghosthaus.x is here
ready to be run.
<br>
The game executable and "episodes" folder must be in the same directory.

## Compiling the game

<br>
The game must be compiled with -lncursesw linker flag, which requires ncurses
to be installed as it is not a default library.
Also the C++ version has to be set to c++ 17 or newer.

<br>

### Makefile:  
`make spooky` - to compile the game  
`make clean`  - to erase the compiled game  

### Copypaste:  
`g++ main.cpp includes/*.cpp includes/class/*.cpp includes/logic/*.cpp includes/singleton/*.cpp -o ghosthaus.x -Wall -lncursesw -std=c++17`  

Once compiled, an executable ghostaus.x will be ready to be run.

--------------------------------------------------------------------------------
## Terminal info:

**Terminal of at least 80 columns by 24 rows is required.**  
(preferably about 120 columns by 34 rows for best experience)  

Colored terminal is recommended. It will run on a non colored one, but the
experience will be far from desirable.    

If game fails to set font to unicode it won't run.    

Resizing terminal window is not a good idea, although the game corrects itself
when exiting the title screen to start the game, so you can resize the terminal
just before pressing NEW GAME, selecting an episode or going back to title
screen.

Game has been developed in WSL Ubuntu, tested in WSL Ubuntu and Fedora.  
Should any compatibility issues surface let me know.

Recommended fonts:  
On WSL- Consolas and (maybe) Courier New are the best fonts for everything to
display properly.
Consolas looks very nice when bold, unlike Courier New.

This game uses bold version of color black to represent the color gray, which
is being used very often here. On most terminals it looks as intended, however
some may be set to represent bold black as just... bolder version of black.
<br>
To fix it, You can either try to change the terminal emulator settings to
display bold black differently, or You can change all apprearances of color
gray to cyan in the game's settings.

--------------------------------------------------------------------------------
## Run flags:

There are optional run flags the game recognizes:  

- **debug**<br>
`-d` or `-debug`<br><br>
![Short footage demonstrating the debug flag](/media/movie_prez_debug.gif)
Displays some info on top of the screen and grands access to some handy debug features, such as commands and debugger log.<br><br>

- **no delay**<br>
`-nd` or `-nodelay`<br><br>
![Short footage demonstrating the nodelay flag](/media/movie_prez_nodelay.gif)
Cancels a slight delay that happens every turn before input, so  you can zoom through the level without much control.<br><br>
    

----------------------------------------------------------------------------
----
## Game controls:

**Arrow keys**  
movement, interacting with environment, selecting things  
**Enter**       
confirming things  
**Page Up**     
inventory previous page (when multiple pages are available)  
**Page Down**   
inventory next page (when multiple pages are available) 
**D**           
drop and item  
**C**           
use the camera flash  
**S**           
use the stopwatch  
**F7**          
exit to title screen  
**F10**         
exit the game  
**Space**       
"skip turn" (when debug is enabled)  
**: (colon key)**           
enter command mode (when debug is enabled)  

--------------------------------------------------------------------------------
## Debug commands:

**:give <item\>**  
  where <item\> can be:
  green key, red key, flashlight, camera, super flashlight, skeleton key
  stopwatch, blue gem, red gem, green gem  
**:light up**, **:light down**  
  (manipulates the field of view)  
**:wq**, **:q!**, **:quit**, **:exit**  
  (exits the game)     
**:die**  
  (does what you expect)  
**:invincible**  
  (toggles invincibility, you can not be killed by an enemy if invincible)

-------------------------
## Changelog

### [1.1] - 2023-06-5
#### Added
- Debug logs: when debug is enabled, a log will be automatically generated


#### Changes
- Major code refactor
- Made README.md 150% cooler
- Main action window border is now 50% more spooky
- Messages displayed during gameplay events are now centered
- Minor changes to level layouts

### [1.0] - 2022-09-11
#### Added
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
Note: versions below 1.0 are actually milestones and not previous commits. I made them to break up the task of creating the game into smaller pieces and to get some feedback from my friends.

### [beta 2] - 2022-09-06
#### Added
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

### [beta 1.1] - 2022-09-03
#### Changes
- Attempting to fix one very specific bug in render

### [beta 1] - 2022-09-03
#### Added
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

### [indev 3] - 2022-08-28
#### Added
- Ghost enemy (finally)
- New items (camera, skeleton key, super flashlight)
- Working camera by pressing C, it creates a flash that stuns all enemies for a couple turns
- Guard enemies
- Switchable walls, switchable floor
- New switch object, that turns switchable walls to switchable floors and vice versa when targeted by player
- More debug information
- There are more possible flashlight levels than just two now
- Changes to tiles and maps
- Changes in level layouts
- Minor aesthetic changes

- Commands
  Reworked the vim easter egg into working commands, if debug mode is enabled
  you can press : to start typing a command:
  :give item (green key, flashlight, camera, super flashlight, skeleton key)
  :light up, :light down
  :wq, :q!, :quit, :exit

#### Important improvements
- Fixed an annoying problem with input stacking uncontrollably


### [indev 2] - 2022-08-16
#### Added
- Dropping items witch D ( select an item to drop with arrow keys, then drop
  highlighted item with Enter or cancel with D ( sadly I can't use Esc ) )
- Completely new episode system with .ep files
- Completely new episode select screen
- New episode "GAME TESTER'S DELIGHT"
- Title screen SETTINGS page with option to select how to represent gray color
- Minor changes to tiles and maps
- Minor aesthetic changes (red arrows on title screen ect)
- Changes in level layouts
- A little vim easter egg

### [indev 1] - 2022-08-12
#### Added
- 100% of things that are currently here, most of them implemented in a rush when I thought I only have a couple days before the deadline
