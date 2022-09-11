-> Creating episodes and levels

The game allows for creating custom levels, and even custom sets of levels,
called "episodes".

The detailed guide will be provided below, however there is one rule that
applies everywhere:

This game is made to run in terminal of any size, as long as it is at least 80
columns wide and 24 rows heigh. Keeping this in mind, every time there is a
text message that is supposed to be displayed in one line, it should be at most
80 characters long. Furthermore, because of how the game is stylized, every
time a message is to be outputted to the box at the bottom of the screen
(messages from signs and enemies), it should be at most 70 characters long.
Longer messages won't crash the game, but it'll look stupid.

################################################################################
                              CREATING EPISODES
################################################################################

Creating a level comes down to creating a special text file with .ep file
extension. This way the game knows to treat it in a special way.
The file should be located in /episodes/ folder, relative to main executable.

The structure of the file is as follows:

EPISODE NAME
/folder/path/
inventory_size
intro dialogue line 1
intro dialogue line 2
(...)
intro dialogue line n

Where:
EPISODE NAME		is the thing that can be seen in episode select screen
/folder/path/		is the path to the folder that has all the levels' data
			inside (relative to /episodes/ folder)
inventory_size		is a number and is self explanatory
intro dialogue lines	are the lines of text that display one by one when you
			start the episode, their number is not specified
			anywhere as every line from line 4 onward will count.

As an example, this is how spooky_house_adventure.ep looks like:
================================================================================
~ SPOOKY HOUSE ADVENTURE ~
/spooky house adventure/
5
You have been unconscious for god knows how long.
After a deep dark slumber, you awake in the middle of nowhere.
You find yourself lying on a table.
Barely seeing anything, you gather strength to get up...
================================================================================

The code responsible for reading the .ep file is not foolproof, so be careful to
write it carefully. No line breaks or leading spaces are tolerated.
However, given how little the file contains, who cares.

Once you have your .ep file ready, you can select your episode from the title
screen and the game will try to locate 1.txt in the specified folder.
1.txt is the only level mandatory to be in the folder. After the player beats
this first level, the game will attempt to find another one (2.txt, 3.txt and so
on), and in case of failure, the episode will just end with a nice final score
message.

If there is a gap in level numbers, for example the folder contains 1.txt,
2.txt, 4.txt but not 3.txt, the game will assume the episode is over anyway
after finishing 2.txt.

################################################################################
                               CREATING LEVELS
################################################################################

You have much more freedom while creating levels than .ep files.
Each level is represented as a normal (.txt) text file, which's name is the
number of the level (1.txt, 2.txt, 3.txt, and so on).
The structure of the level file has a "head" and a "body" of sorts.

The "head" part contains parameters and custom settings for the level, from now
on i will call them #.
All # have a structure like this

#parameter/setting
<value>
<in rare occasions - value 2>

And here are all possible # and their arguments:

#width			specifies the width of the board
<number>

#height			specifies the height of the board
<number>

#floor_color		sets the level floor color depending on <value>, where
<value>			0, black, BLACK or COLOR_BLACK results in black floor
			1, blue, BLUE or COLOR_BLUE results in blue floor and
			2, magenta, MAGENTA or COLOR_MAGENTA you get the drill
			(instead of #floor_color you can write #floor)

#sign n			sets the n'th sign message to <message> (1 <= n <= 9)
<message>

#ghost_level		sets the craziness level of the ghosts, default being
<number>		1, 0 being the lowest possible.
			(instead of #ghost_level you can write #ghost or
			#ghost_lvl)

#guard_killmessage n	overrides the n'th guard's message, which displays
<message>		after it kills you (1 <= n <= 4)

#guard_direction n	overrides the n'th guard's start direction (for
<number_x>		example 0 as <number_x> and 1 as <number_y> will result
<number_y>		in the guard walking down) (1 <= n <= 4)

#guard_mode n		overrides the n'th guard's behaviour after it meets a
<number>		solid object, where <number>:
			0 - reverses direction, 1 - rotates direction clockwise
			2 - rotates direction counter clockwise
			(1 <= n <= 4)

#level_end_message	overrides level end message with <message>
<message>		(instead of #level_end_message you can #end_message)

#death_message		overrides level death message with <message>
<message>

#data			marks the end of "head" part
			(instead of #data you can write ##data)


As a general rule, every # in the "head" part can be put here in any order,
they can be separated by line breaks for better readability or may just contain
random garbage between them, as any line that doesn't match any # or is not
directly below it will be ignored
(however, there cannot be any line breaks or anything between a # and it's
values).
An exception to this rule is #data, which must be the last # and is the
beginning of the "body" part.

Also, all # are optional, except for #width, #height and #data.

The #width and #height must have positive values, and may be artificially
enlarged by the program depending on how big your terminal window is.

There are four possible guard enemy types to override:
1 - red guard that starts in frame 0, goes left and has mode 0.
    "Wham! You are dead!"
2 - red guard that starts in frame 1, goes up and has mode 0.
	"Wham! You are done for!"
3 - green guard that starts in frame 0, goes left and has mode 2.
	"Wham! You didn't stand a chance!"
4 - green guard that starts in frame 1, goes right and has mode 1.
	"Wham! You are unalived!"

There are also nine signs to set messages for, by default they have no message.

As an example, this is how /spooky house adventure/1.txt body part looks like:
================================================================================
#sign 1
Danger ahead! Beware of the ghost!
#sign 2
Oh boy, I love my key that unlocks a green door, hope nobody steals it

#death_message
Okay, how did this happen?

#width
36
#height
34

#data
================================================================================

Now, for the "body part"
The "body" part begins after #data and is the last part of .txt file. No # will be
accepted after #data. This is where the level layout lies.
Once the program finds #data, it will try to fetch y lines of x characters,
where y and x have been previously set by #height and #width. If there are less
than y lines left, or some line contains less than x characters, the game will
fill the missing space with void tiles. If there are more lines or characters
than necessary, they will simply be ignored.

Here is a table of how all characters are being interpreted.
The spacial relations between the characters will translate to spacial relations
between tiles and entities (if char a is to the left of char b, the resulting
tile a will be to the left of tile b).

Useless information		Useful information
--------------------------------------------------------------------------------
Dec	Hex	Oct		Char	Effect
--------------------------------------------------------------------------------
32	20	040		Space	tile: floor
33	21	041		!	object: switch
34	22	042		"
35	23	043		#	tile: brick wall
36	24	044		$	tile: fake brick wall
37	25	045		%
38	26	046		&
39	27	047		'
40	28	050		(
41	29	051		)
42	2A	052		*
43	2B	053		+	tile: grave
44	2C	054		,	tile: grass
45	2D	055		-
46	2E	056		.	tile: path
47	2F	057		/
48	30	060		0	tile: void
49	31	061		1	tile: sign 1
50	32	062		2	tile: sign 2	
51	33	063		3	tile: sign 3
52	34	064		4	tile: sign 4
53	35	065		5	tile: sign 5
54	36	066		6	tile: sign 6
55	37	067		7	tile: sign 7
56	38	070		8	tile: sign 8
57	39	071		9	tile: sign 9
58	3A	072		:
59	3B	073		;
60	3C	074		<
61	3D	075		=
62	3E	076		>
63	3F	077		?
64	40	100		@	tile: switch wall
65	41	101		A	item: skeleton(almighty) key
66	42	102		B
67	43	103		C	item: camera
68	44	104		D	object: door(simple, unlocked)
69	45	105		E	level exit
70	46	106		F	item: flashlight
71	47	107		G	enemy: ghost
72	48	110		H
73	49	111		I
74	4A	112		J
75	4B	113		K	item: green key
76	4C	114		L	item: red key
77	4D	115		M	object: green door (locked)
78	4E	116		N	object: red door (locked)
79	4F	117		O
80	50	120		P	player starting position
81	51	121		Q
82	52	122		R
83	53	123		S	item: stopwatch
84	54	124		T	tile: tree bottom, but also replaces the
					tile above with tree top
85	55	125		U
86	56	126		V
87	57	127		W
88	58	130		X
89	59	131		Y
90	5A	132		Z
91	5B	133		[
92	5C	134		\
93	5D	135		]
94	5E	136		^
95	5F	137		_	tile: switch floor
96	60	140		`	tile: floor, but also replaces the tile
					above with a table tile (useful if you
					want to have an item lying on a table)
97	61	141		a	enemy: guardian 1 (by default, starts
					left and reverses direction on contact
					with a wall)
98	62	142		b	enemy: guardian 2 (by default, starts
					up and reverses direction on contact
					with a wall)
99	63	143		c	enemy: guardian 3 (by default, starts
					left and rotates counter clockwise on
					contact with a wall)
100	64	144		d	enemy: guardian 4 (by default, starts
					right and rotates clockwise on contact
					with a wall)
101	65	145		e
102	66	146		f	tile: fake rock
103	67	147		g	item: blue gemstone (worth 1000)
104	68	150		h	item: red gemstone (worth 500)
105	69	151		i	item: green gemstone (worth 200)
106	6A	152		j
107	6B	153		k
108	6C	154		l
109	6D	155		m
110	6E	156		n
111	6F	157		o
112	70	160		p	tile: pillar
113	71	161		q
114	72	162		r	tile: rock
115	73	163		s
116	74	164		t	tile: table
117	75	165		u
118	76	166		v
119	77	167		w	tile: water
120	78	170		x
121	79	171		y
122	7A	172		z
123	7B	173		{
124	7C	174		|
125	7D	175		}
126	7E	176		~	tile: red water

Using any character not described here will result in placing a void tile, as if
it was replaced by 0.

There is no limit to how many entities, objects and tiles can be placed in one
level. For example, you can place 15 different ghosts if you wish to.
There are, however, two exceptions to this rule: player and exit.
There is always a single instance of them (initially at x = 0, y = 0) and their
position updates if the algorithm finds their letter (P or E), meaning that
the lowest lines (and rightmost characters in the same line) take priority.

Since the exit logic is disconnected from exit visual, you can abuse this
to create fake, non working exits.
Or, since updating players's position also places a floor tile below, I guess
you can use players to put floor tiles around in a fancy way that will be
super unreadable when looked at. I don't judge.

Have fun! or something
