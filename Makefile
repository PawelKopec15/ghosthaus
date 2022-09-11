DIR = $(notdir $(CURDIR))

FLAGS = -Wall -lncursesw -std=c++17

## keeping these as comments, maybe they will be useful later
##
## INC1 = includes/gh_simple_functions.cpp includes/gh_class_positionobject.cpp includes/gh_class_visibleentity.cpp includes/gh_class_sign.cpp includes/gh_class_item.cpp
## INC2 = includes/gh_class_tile.cpp includes/gh_constants_and_colors.cpp includes/gh_class_inventory.cpp includes/gh_class_camera.cpp includes/gh_class_player.cpp
## INC3 = includes/gh_classes_enemies.cpp includes/gh_classes_tilemanipulators.cpp includes/gh_game_logic.cpp includes/gh_game_routines.cpp includes/gh_windows.cpp
## INC4 = includes/gh_game_scenes.cpp includes/gh_ghosthaus.cpp
##COMMAND = g++ main.cpp $(INC1) $(INC2) $(INC3) $(INC4) -o ghosthaus.x $(FLAGS)

INCLUDES_FOLDER = includes

COMMAND = g++ main.cpp $(INCLUDES_FOLDER)/*.cpp -o ghosthaus.x $(FLAGS)

.PHONY: all spooky clean

all:
	$(COMMAND)

spooky:
	$(COMMAND)

clean:
	rm -f *.x