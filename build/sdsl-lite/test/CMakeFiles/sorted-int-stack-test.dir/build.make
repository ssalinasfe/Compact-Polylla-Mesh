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

# Utility rule file for sorted-int-stack-test.

# Include the progress variables for this target.
include sdsl-lite/test/CMakeFiles/sorted-int-stack-test.dir/progress.make

sdsl-lite/test/CMakeFiles/sorted-int-stack-test: sdsl-lite/test/sorted_int_stack_test
sdsl-lite/test/CMakeFiles/sorted-int-stack-test: sdsl-lite/test/sorted_int_stack_test
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Execute sorted-int-stack-test."
	/home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test/sorted_int_stack_test /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/tmp

sorted-int-stack-test: sdsl-lite/test/CMakeFiles/sorted-int-stack-test
sorted-int-stack-test: sdsl-lite/test/CMakeFiles/sorted-int-stack-test.dir/build.make

.PHONY : sorted-int-stack-test

# Rule to build all files generated by this target.
sdsl-lite/test/CMakeFiles/sorted-int-stack-test.dir/build: sorted-int-stack-test

.PHONY : sdsl-lite/test/CMakeFiles/sorted-int-stack-test.dir/build

sdsl-lite/test/CMakeFiles/sorted-int-stack-test.dir/clean:
	cd /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test && $(CMAKE_COMMAND) -P CMakeFiles/sorted-int-stack-test.dir/cmake_clean.cmake
.PHONY : sdsl-lite/test/CMakeFiles/sorted-int-stack-test.dir/clean

sdsl-lite/test/CMakeFiles/sorted-int-stack-test.dir/depend:
	cd /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test/CMakeFiles/sorted-int-stack-test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sdsl-lite/test/CMakeFiles/sorted-int-stack-test.dir/depend

