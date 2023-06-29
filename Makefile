DIR = $(notdir $(CURDIR))

FLAGS = -O2 -Wall -lncursesw -std=c++17

# INCLUDES_FOLDER = includes
# CLASS_FOLDER = $(INCLUDES_FOLDER)/class
# LOGIC_FOLDER = $(INCLUDES_FOLDER)/logic
# SINGLETON_FOLDER = $(INCLUDES_FOLDER)/singleton

# INC = $(INCLUDES_FOLDER)/*.cpp $(CLASS_FOLDER)/*.cpp $(LOGIC_FOLDER)/*.cpp $(SINGLETON_FOLDER)/*.cpp

# COMMAND = g++ main.cpp $(INC) -o ghosthaus.x $(FLAGS)

SOURCE_FOLDER = src

# # TODO: later to be replaced with src/ subdirectories
# INCLUDES_FOLDER = includes
# # TODO: down here \/
# UTILITY_FOLDER = $(INCLUDES_FOLDER)/utility
# CURSESWRAPPER_FOLDER = $(INCLUDES_FOLDER)/curseswrapper
# SINGLETON_FOLDER = $(INCLUDES_FOLDER)/singleton
# MASTER_FOLDER = $(INCLUDES_FOLDER)/master

# INCLUDES_COMMAND = $(UTILITY_FOLDER)/*.hpp 


COMMAND = g++ $(SOURCE_FOLDER)/ghosthaus.cpp -o ghosthaus.x $(FLAGS)

.PHONY: all spooky clean

all:
	$(COMMAND)

spooky:
	$(COMMAND)

clean:
	rm -f *.x