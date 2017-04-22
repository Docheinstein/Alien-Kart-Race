SFML_PATH = /usr/include/SFML/
SFML_GRAPHICS_NAME = sfml-graphics
SFML_WINDOW_NAME = sfml-window
SFML_SYSTEM_NAME = sfml-system

BIN_PATH = ./bin/
SRC_PATH = ./src/
TRASH_PATH = ./.trash/

DEBUGGER = nemiver
DEBUG_FLAG = -g
WARNING_FLAG = -Wall

# Dependencies file location
DEPENDENCIES_FILE = $(TRASH_PATH)dependencies

# Retrieve the subdir of src folder
INCLUDE_DIRS = $(shell find $(SRC_PATH) -type d)

# Prepend -I to subdirs
INCLUDE_PARAMS = $(foreach d, $(INCLUDE_DIRS), -I$d)

# Retrieve the cc files
SRC_FILES = $(shell find $(SRC_PATH) | grep "\.cc")

# Assume that foreach cc files there is an object file to generate
# Do this by replacing .cc with .o
OBJS = $(SRC_FILES:.cc=.o)

CXXFLAGS = $(INCLUDE_PARAMS) $(DEBUG_FLAG) $(WARNING_FLAG)

#OBJS = \
#	$(SRC_PATH)main.o\
#	$(SRC_PATH)game/game.o\
#	$(SRC_PATH)map/map.o\
#OBJS = $(patsubst %.cc, $(OBJDIR)/%o, $(SRC_PATH))

.PHONY: compile \
		compile-release \
		compile-debug \
		enable-compile-debug \
		disable-compile-debug

# [DEFAULT_TARGET]
# Compile in release mode
release: disable-compile-debug main

# Compile in debug mode
debug: enable-compile-debug main

# Add -g flag
enable-compile-debug:
	DEBUG_MODE=-g

# Remove -g flag
disable-compile-debug:
	DEBUG_MODE=

main:	$(OBJS)
	g++ $(OBJS) -o $(BIN_PATH)main -L $(SFML_PATH)\
	 -l$(SFML_GRAPHICS_NAME) -l$(SFML_WINDOW_NAME) -l$(SFML_SYSTEM_NAME)\
	 $(DEBUG_FLAG) $(WARNING_FLAG)

.PHONY: clean run run-release run-debug
clean:
	find . -name '*.o' -delete

# Helper for run in release mode
run: run-release

# Run in release mode
run-release:
	$(BIN_PATH)main

# Run using a debugger
run-debug:
	$(DEBUGGER) $(BIN_PATH)main

%.o: %.cc
	g++ $(CXXFLAGS) -c $< -o $@
