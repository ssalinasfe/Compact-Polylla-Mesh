# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_SOURCE_DIR = /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build

# Utility rule file for moby.int.

# Include the progress variables for this target.
include sdsl-lite/test/CMakeFiles/moby.int.dir/progress.make

sdsl-lite/test/CMakeFiles/moby.int: ../sdsl-lite/test/test_cases/moby.int


../sdsl-lite/test/test_cases/moby.int:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Checking or downloading test case moby.int."
	cd /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test && /usr/bin/cmake -Ddownload_url=http://algo2.iti.kit.edu/gog/sdsl/moby.int.tar.gz -Dtest_case_dir=/home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/test_cases -P /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/download.cmake

moby.int: sdsl-lite/test/CMakeFiles/moby.int
moby.int: ../sdsl-lite/test/test_cases/moby.int
moby.int: sdsl-lite/test/CMakeFiles/moby.int.dir/build.make

.PHONY : moby.int

# Rule to build all files generated by this target.
sdsl-lite/test/CMakeFiles/moby.int.dir/build: moby.int

.PHONY : sdsl-lite/test/CMakeFiles/moby.int.dir/build

sdsl-lite/test/CMakeFiles/moby.int.dir/clean:
	cd /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test && $(CMAKE_COMMAND) -P CMakeFiles/moby.int.dir/cmake_clean.cmake
.PHONY : sdsl-lite/test/CMakeFiles/moby.int.dir/clean

sdsl-lite/test/CMakeFiles/moby.int.dir/depend:
	cd /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test/CMakeFiles/moby.int.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sdsl-lite/test/CMakeFiles/moby.int.dir/depend

