# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon Editor"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon Editor"

# Utility rule file for ContinuousBuild.

# Include any custom commands dependencies for this target.
include lib/includes/yaml-cpp/CMakeFiles/ContinuousBuild.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/includes/yaml-cpp/CMakeFiles/ContinuousBuild.dir/progress.make

lib/includes/yaml-cpp/CMakeFiles/ContinuousBuild:
	cd "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon Editor/lib/includes/yaml-cpp" && /usr/bin/ctest -D ContinuousBuild

ContinuousBuild: lib/includes/yaml-cpp/CMakeFiles/ContinuousBuild
ContinuousBuild: lib/includes/yaml-cpp/CMakeFiles/ContinuousBuild.dir/build.make
.PHONY : ContinuousBuild

# Rule to build all files generated by this target.
lib/includes/yaml-cpp/CMakeFiles/ContinuousBuild.dir/build: ContinuousBuild
.PHONY : lib/includes/yaml-cpp/CMakeFiles/ContinuousBuild.dir/build

lib/includes/yaml-cpp/CMakeFiles/ContinuousBuild.dir/clean:
	cd "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon Editor/lib/includes/yaml-cpp" && $(CMAKE_COMMAND) -P CMakeFiles/ContinuousBuild.dir/cmake_clean.cmake
.PHONY : lib/includes/yaml-cpp/CMakeFiles/ContinuousBuild.dir/clean

lib/includes/yaml-cpp/CMakeFiles/ContinuousBuild.dir/depend:
	cd "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon Editor" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon Editor" "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon 2.0/includes/yaml-cpp" "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon Editor" "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon Editor/lib/includes/yaml-cpp" "/home/alexis/Documents/dev/c++/Armageddon-2.0/Armageddon Editor/lib/includes/yaml-cpp/CMakeFiles/ContinuousBuild.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : lib/includes/yaml-cpp/CMakeFiles/ContinuousBuild.dir/depend

