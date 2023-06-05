DIR = $(notdir $(CURDIR))

FLAGS = -Wall -lncursesw -std=c++17

INCLUDES_FOLDER = includes
CLASS_FOLDER = $(INCLUDES_FOLDER)/class
LOGIC_FOLDER = $(INCLUDES_FOLDER)/logic
SINGLETON_FOLDER = $(INCLUDES_FOLDER)/singleton

INC = $(INCLUDES_FOLDER)/*.cpp $(CLASS_FOLDER)/*.cpp $(LOGIC_FOLDER)/*.cpp $(SINGLETON_FOLDER)/*.cpp

COMMAND = g++ main.cpp $(INC) -o ghosthaus.x $(FLAGS)

.PHONY: all spooky clean

all:
	$(COMMAND)

spooky:
	$(COMMAND)

clean:
	rm -f *.x