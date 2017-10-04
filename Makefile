# Path of the SFML library
SFML_PATH = /usr/include/SFML/

# Name of SFML components
SFML_GRAPHICS_NAME = sfml-graphics
SFML_WINDOW_NAME = sfml-window
SFML_SYSTEM_NAME = sfml-system

# Path for the binaries
BIN_PATH = ./bin/

# Path for the sources
SRC_PATH = ./src/

# Debugger used when run with  "make run-debug"
DEBUGGER = nemiver

DEBUG_OUTPUT_DEFINE_NAME = DEBUG_OUTPUT_LEVEL_MASK
DEBUG_FLAG = -g
WARNING_FLAG = -Wall

DEBUG_MODE =
DEBUG_OUTPUT_LEVEL = -D$(DEBUG_OUTPUT_DEFINE_NAME)=0

# STD 11 is used by the project for different features
# like variadic templates (for the logger) and initialization
# lists for structs.
# SFML however is compiled withouth c++11 support, so disable
# the compiler ABI is needed for accomodates both.
STD_11_FLAG = -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0

# Retrieves the subdirectories of the sources folder
ifeq ($(OS),Windows_NT)
# On WIN
	INCLUDE_DIRS = $(shell dir $(SRC_PATH) /a:d /s /b | sort)
else
# On LINUX
	INCLUDE_DIRS = $(shell find $(SRC_PATH) -type d)
endif

# Composes the include params by prepending -I to the retrieved subdirectories
INCLUDE_PARAMS = $(foreach d, $(INCLUDE_DIRS), -I$d)

# Retrieves the .cc files from the sources folder searching them recursively
ifeq ($(OS),Windows_NT)
# On WIN
	SRC_FILES = $(shelldir *.cc /b/s)
else
# On LINUX
	SRC_FILES = $(shell find $(SRC_PATH) | grep "\.cc")
endif

# The objects files are composed by supposing that for each .cc file
# a .o file should be generated with the same name
OBJS = $(SRC_FILES:.cc=.o)

# Flags used by the C++ compiler
CXXFLAGS = $(STD_11_FLAG) $(INCLUDE_PARAMS) $(WARNING_FLAG)

# --------------------------------------------
# ------------ COMPILATION TARGETS -----------
# --------------------------------------------
.PHONY: release \
		debug \
		debug1 \
		debug2 \
		disable-compile-debug \
		enable-compile-debug

# [DEFAULT TARGET]
# Compiles in release mode
release: CXXFLAGS += -D$(DEBUG_OUTPUT_DEFINE_NAME)=0
release: main

# Compiles in debug mode with default verbosity (1)
debug: debug1

# Compiles in debug mode with verbosity set to 1
debug1: CXXFLAGS += -D$(DEBUG_OUTPUT_DEFINE_NAME)=1 $(DEBUG_FLAG)
debug1: main

# Compiles in debug mode with verbosity set to 2
debug2: CXXFLAGS += -D$(DEBUG_OUTPUT_DEFINE_NAME)=3 $(DEBUG_FLAG)
debug2: main

# Main target
main:	$(OBJS)
	g++ $(CXXFLAGS) $(OBJS) -o $(BIN_PATH)main -L $(SFML_PATH)\
	 -l$(SFML_GRAPHICS_NAME) -l$(SFML_WINDOW_NAME) -l$(SFML_SYSTEM_NAME)\

# For each object file takes includes the dependencies from its dependency file
DEPENDENCIES = $(OBJS:.o=.d)
-include $(DEPENDENCIES)

# --------------------------------------------
# -----------RUN TARGETS ---------------------
# --------------------------------------------
.PHONY: run \
		run-release \
		run-debug

# Helper alias for run-release
run: run-release

# Runs the project in release mode
run-release:
	$(BIN_PATH)main

# Runs the project using the specified debugger
run-debug:
	$(DEBUGGER) $(BIN_PATH)main

# --------------------------------------------
# -----------UTILITY TARGETS -----------------
# --------------------------------------------
.PHONY: clean \
		clean-all

# Removes every object, dependency and binary file
clean-all: clean
	rm $(BIN_PATH)main

# Removes every object and dependency file
clean:
	find . -name '*.o' -delete
	find . -name '*.d' -delete

# --------------------------------------------
# -----------END UTILITY TARGETS -------------
# --------------------------------------------

# Compiles the objects files by using the .cc file with the same name.
# At the same time creates a dependency file (.d) in the same directory
# of the .cc that contains its dependencies.
# I opt for the .d approch instead of using a unique dependency file
# since the project is placed within multiple directory, and an unique
# depend file is not much modular.
# Furthermore creates the .d files is needed, otherwise when a header file changes
# and its object file is already built, the change will be ignored.
# (The -MMD flag creates the .d with the same name as the .cc)
%.o: %.cc
	g++ $(CXXFLAGS) -c $< -o $@ -MMD