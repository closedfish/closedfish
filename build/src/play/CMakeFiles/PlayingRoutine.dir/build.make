# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/andrei/Desktop/Switch AI"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/andrei/Desktop/Switch AI/build"

# Include any dependencies generated for this target.
include src/play/CMakeFiles/PlayingRoutine.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/play/CMakeFiles/PlayingRoutine.dir/compiler_depend.make

# Include the progress variables for this target.
include src/play/CMakeFiles/PlayingRoutine.dir/progress.make

# Include the compile flags for this target's objects.
include src/play/CMakeFiles/PlayingRoutine.dir/flags.make

src/play/CMakeFiles/PlayingRoutine.dir/play_main.cc.o: src/play/CMakeFiles/PlayingRoutine.dir/flags.make
src/play/CMakeFiles/PlayingRoutine.dir/play_main.cc.o: ../src/play/play_main.cc
src/play/CMakeFiles/PlayingRoutine.dir/play_main.cc.o: src/play/CMakeFiles/PlayingRoutine.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/andrei/Desktop/Switch AI/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/play/CMakeFiles/PlayingRoutine.dir/play_main.cc.o"
	cd "/home/andrei/Desktop/Switch AI/build/src/play" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/play/CMakeFiles/PlayingRoutine.dir/play_main.cc.o -MF CMakeFiles/PlayingRoutine.dir/play_main.cc.o.d -o CMakeFiles/PlayingRoutine.dir/play_main.cc.o -c "/home/andrei/Desktop/Switch AI/src/play/play_main.cc"

src/play/CMakeFiles/PlayingRoutine.dir/play_main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PlayingRoutine.dir/play_main.cc.i"
	cd "/home/andrei/Desktop/Switch AI/build/src/play" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/andrei/Desktop/Switch AI/src/play/play_main.cc" > CMakeFiles/PlayingRoutine.dir/play_main.cc.i

src/play/CMakeFiles/PlayingRoutine.dir/play_main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PlayingRoutine.dir/play_main.cc.s"
	cd "/home/andrei/Desktop/Switch AI/build/src/play" && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/andrei/Desktop/Switch AI/src/play/play_main.cc" -o CMakeFiles/PlayingRoutine.dir/play_main.cc.s

# Object files for target PlayingRoutine
PlayingRoutine_OBJECTS = \
"CMakeFiles/PlayingRoutine.dir/play_main.cc.o"

# External object files for target PlayingRoutine
PlayingRoutine_EXTERNAL_OBJECTS =

src/play/libPlayingRoutine.a: src/play/CMakeFiles/PlayingRoutine.dir/play_main.cc.o
src/play/libPlayingRoutine.a: src/play/CMakeFiles/PlayingRoutine.dir/build.make
src/play/libPlayingRoutine.a: src/play/CMakeFiles/PlayingRoutine.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/andrei/Desktop/Switch AI/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libPlayingRoutine.a"
	cd "/home/andrei/Desktop/Switch AI/build/src/play" && $(CMAKE_COMMAND) -P CMakeFiles/PlayingRoutine.dir/cmake_clean_target.cmake
	cd "/home/andrei/Desktop/Switch AI/build/src/play" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PlayingRoutine.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/play/CMakeFiles/PlayingRoutine.dir/build: src/play/libPlayingRoutine.a
.PHONY : src/play/CMakeFiles/PlayingRoutine.dir/build

src/play/CMakeFiles/PlayingRoutine.dir/clean:
	cd "/home/andrei/Desktop/Switch AI/build/src/play" && $(CMAKE_COMMAND) -P CMakeFiles/PlayingRoutine.dir/cmake_clean.cmake
.PHONY : src/play/CMakeFiles/PlayingRoutine.dir/clean

src/play/CMakeFiles/PlayingRoutine.dir/depend:
	cd "/home/andrei/Desktop/Switch AI/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/andrei/Desktop/Switch AI" "/home/andrei/Desktop/Switch AI/src/play" "/home/andrei/Desktop/Switch AI/build" "/home/andrei/Desktop/Switch AI/build/src/play" "/home/andrei/Desktop/Switch AI/build/src/play/CMakeFiles/PlayingRoutine.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : src/play/CMakeFiles/PlayingRoutine.dir/depend
