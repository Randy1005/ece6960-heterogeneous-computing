# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/randy/ece6960-PA1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/randy/ece6960-PA1/build

# Include any dependencies generated for this target.
include threadpool/CMakeFiles/threadpool.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include threadpool/CMakeFiles/threadpool.dir/compiler_depend.make

# Include the progress variables for this target.
include threadpool/CMakeFiles/threadpool.dir/progress.make

# Include the compile flags for this target's objects.
include threadpool/CMakeFiles/threadpool.dir/flags.make

threadpool/CMakeFiles/threadpool.dir/threadpool.cpp.o: threadpool/CMakeFiles/threadpool.dir/flags.make
threadpool/CMakeFiles/threadpool.dir/threadpool.cpp.o: /home/randy/ece6960-PA1/threadpool/threadpool.cpp
threadpool/CMakeFiles/threadpool.dir/threadpool.cpp.o: threadpool/CMakeFiles/threadpool.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/randy/ece6960-PA1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object threadpool/CMakeFiles/threadpool.dir/threadpool.cpp.o"
	cd /home/randy/ece6960-PA1/build/threadpool && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT threadpool/CMakeFiles/threadpool.dir/threadpool.cpp.o -MF CMakeFiles/threadpool.dir/threadpool.cpp.o.d -o CMakeFiles/threadpool.dir/threadpool.cpp.o -c /home/randy/ece6960-PA1/threadpool/threadpool.cpp

threadpool/CMakeFiles/threadpool.dir/threadpool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/threadpool.dir/threadpool.cpp.i"
	cd /home/randy/ece6960-PA1/build/threadpool && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/randy/ece6960-PA1/threadpool/threadpool.cpp > CMakeFiles/threadpool.dir/threadpool.cpp.i

threadpool/CMakeFiles/threadpool.dir/threadpool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/threadpool.dir/threadpool.cpp.s"
	cd /home/randy/ece6960-PA1/build/threadpool && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/randy/ece6960-PA1/threadpool/threadpool.cpp -o CMakeFiles/threadpool.dir/threadpool.cpp.s

# Object files for target threadpool
threadpool_OBJECTS = \
"CMakeFiles/threadpool.dir/threadpool.cpp.o"

# External object files for target threadpool
threadpool_EXTERNAL_OBJECTS =

threadpool/libthreadpool.a: threadpool/CMakeFiles/threadpool.dir/threadpool.cpp.o
threadpool/libthreadpool.a: threadpool/CMakeFiles/threadpool.dir/build.make
threadpool/libthreadpool.a: threadpool/CMakeFiles/threadpool.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/randy/ece6960-PA1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libthreadpool.a"
	cd /home/randy/ece6960-PA1/build/threadpool && $(CMAKE_COMMAND) -P CMakeFiles/threadpool.dir/cmake_clean_target.cmake
	cd /home/randy/ece6960-PA1/build/threadpool && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/threadpool.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
threadpool/CMakeFiles/threadpool.dir/build: threadpool/libthreadpool.a
.PHONY : threadpool/CMakeFiles/threadpool.dir/build

threadpool/CMakeFiles/threadpool.dir/clean:
	cd /home/randy/ece6960-PA1/build/threadpool && $(CMAKE_COMMAND) -P CMakeFiles/threadpool.dir/cmake_clean.cmake
.PHONY : threadpool/CMakeFiles/threadpool.dir/clean

threadpool/CMakeFiles/threadpool.dir/depend:
	cd /home/randy/ece6960-PA1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/randy/ece6960-PA1 /home/randy/ece6960-PA1/threadpool /home/randy/ece6960-PA1/build /home/randy/ece6960-PA1/build/threadpool /home/randy/ece6960-PA1/build/threadpool/CMakeFiles/threadpool.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : threadpool/CMakeFiles/threadpool.dir/depend

