# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/YASH/CLionProjects/3-waytrie

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/YASH/CLionProjects/3-waytrie/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/3_waytrie.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/3_waytrie.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/3_waytrie.dir/flags.make

CMakeFiles/3_waytrie.dir/main.c.o: CMakeFiles/3_waytrie.dir/flags.make
CMakeFiles/3_waytrie.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/YASH/CLionProjects/3-waytrie/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/3_waytrie.dir/main.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/3_waytrie.dir/main.c.o   -c /Users/YASH/CLionProjects/3-waytrie/main.c

CMakeFiles/3_waytrie.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/3_waytrie.dir/main.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/YASH/CLionProjects/3-waytrie/main.c > CMakeFiles/3_waytrie.dir/main.c.i

CMakeFiles/3_waytrie.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/3_waytrie.dir/main.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/YASH/CLionProjects/3-waytrie/main.c -o CMakeFiles/3_waytrie.dir/main.c.s

CMakeFiles/3_waytrie.dir/main.c.o.requires:

.PHONY : CMakeFiles/3_waytrie.dir/main.c.o.requires

CMakeFiles/3_waytrie.dir/main.c.o.provides: CMakeFiles/3_waytrie.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/3_waytrie.dir/build.make CMakeFiles/3_waytrie.dir/main.c.o.provides.build
.PHONY : CMakeFiles/3_waytrie.dir/main.c.o.provides

CMakeFiles/3_waytrie.dir/main.c.o.provides.build: CMakeFiles/3_waytrie.dir/main.c.o


# Object files for target 3_waytrie
3_waytrie_OBJECTS = \
"CMakeFiles/3_waytrie.dir/main.c.o"

# External object files for target 3_waytrie
3_waytrie_EXTERNAL_OBJECTS =

3_waytrie: CMakeFiles/3_waytrie.dir/main.c.o
3_waytrie: CMakeFiles/3_waytrie.dir/build.make
3_waytrie: CMakeFiles/3_waytrie.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/YASH/CLionProjects/3-waytrie/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable 3_waytrie"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/3_waytrie.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/3_waytrie.dir/build: 3_waytrie

.PHONY : CMakeFiles/3_waytrie.dir/build

CMakeFiles/3_waytrie.dir/requires: CMakeFiles/3_waytrie.dir/main.c.o.requires

.PHONY : CMakeFiles/3_waytrie.dir/requires

CMakeFiles/3_waytrie.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/3_waytrie.dir/cmake_clean.cmake
.PHONY : CMakeFiles/3_waytrie.dir/clean

CMakeFiles/3_waytrie.dir/depend:
	cd /Users/YASH/CLionProjects/3-waytrie/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/YASH/CLionProjects/3-waytrie /Users/YASH/CLionProjects/3-waytrie /Users/YASH/CLionProjects/3-waytrie/cmake-build-debug /Users/YASH/CLionProjects/3-waytrie/cmake-build-debug /Users/YASH/CLionProjects/3-waytrie/cmake-build-debug/CMakeFiles/3_waytrie.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/3_waytrie.dir/depend

