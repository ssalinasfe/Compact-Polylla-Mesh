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

# Utility rule file for lcp-construct-test_empty.txt.

# Include the progress variables for this target.
include sdsl-lite/test/CMakeFiles/lcp-construct-test_empty.txt.dir/progress.make

sdsl-lite/test/CMakeFiles/lcp-construct-test_empty.txt: ../sdsl-lite/test/test_cases/empty.txt
sdsl-lite/test/CMakeFiles/lcp-construct-test_empty.txt: ../sdsl-lite/test/tmp
sdsl-lite/test/CMakeFiles/lcp-construct-test_empty.txt: sdsl-lite/test/lcp_construct_test
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Execute lcp-construct-test on empty.txt."
	/home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test/lcp_construct_test /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/test_cases/empty.txt /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/tmp/lcp_construct_test_empty.txt /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/tmp

lcp-construct-test_empty.txt: sdsl-lite/test/CMakeFiles/lcp-construct-test_empty.txt
lcp-construct-test_empty.txt: sdsl-lite/test/CMakeFiles/lcp-construct-test_empty.txt.dir/build.make

.PHONY : lcp-construct-test_empty.txt

# Rule to build all files generated by this target.
sdsl-lite/test/CMakeFiles/lcp-construct-test_empty.txt.dir/build: lcp-construct-test_empty.txt

.PHONY : sdsl-lite/test/CMakeFiles/lcp-construct-test_empty.txt.dir/build

sdsl-lite/test/CMakeFiles/lcp-construct-test_empty.txt.dir/clean:
	cd /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test && $(CMAKE_COMMAND) -P CMakeFiles/lcp-construct-test_empty.txt.dir/cmake_clean.cmake
.PHONY : sdsl-lite/test/CMakeFiles/lcp-construct-test_empty.txt.dir/clean

sdsl-lite/test/CMakeFiles/lcp-construct-test_empty.txt.dir/depend:
	cd /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test /home/cuyigoldo/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test/CMakeFiles/lcp-construct-test_empty.txt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sdsl-lite/test/CMakeFiles/lcp-construct-test_empty.txt.dir/depend

