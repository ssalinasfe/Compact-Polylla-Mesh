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
CMAKE_SOURCE_DIR = /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build

# Utility rule file for rmq-test_int-vec.1000000.64.i.

# Include the progress variables for this target.
include sdsl-lite/test/CMakeFiles/rmq-test_int-vec.1000000.64.i.dir/progress.make

sdsl-lite/test/CMakeFiles/rmq-test_int-vec.1000000.64.i: ../sdsl-lite/test/test_cases/int-vec.1000000.64.i
sdsl-lite/test/CMakeFiles/rmq-test_int-vec.1000000.64.i: ../sdsl-lite/test/tmp
sdsl-lite/test/CMakeFiles/rmq-test_int-vec.1000000.64.i: sdsl-lite/test/rmq_test
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Execute rmq-test on int-vec.1000000.64.i."
	/home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test/rmq_test /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/test_cases/int-vec.1000000.64.i /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/tmp/rmq_test_int-vec.1000000.64.i /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/tmp

../sdsl-lite/test/test_cases/int-vec.1000000.64.i: sdsl-lite/test/int_vector_generator
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating test case int-vec.1000000.64.i."
	/home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test/int_vector_generator /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/test_cases/int-vec.1000000.64.i 1000000 64 i

rmq-test_int-vec.1000000.64.i: sdsl-lite/test/CMakeFiles/rmq-test_int-vec.1000000.64.i
rmq-test_int-vec.1000000.64.i: ../sdsl-lite/test/test_cases/int-vec.1000000.64.i
rmq-test_int-vec.1000000.64.i: sdsl-lite/test/CMakeFiles/rmq-test_int-vec.1000000.64.i.dir/build.make

.PHONY : rmq-test_int-vec.1000000.64.i

# Rule to build all files generated by this target.
sdsl-lite/test/CMakeFiles/rmq-test_int-vec.1000000.64.i.dir/build: rmq-test_int-vec.1000000.64.i

.PHONY : sdsl-lite/test/CMakeFiles/rmq-test_int-vec.1000000.64.i.dir/build

sdsl-lite/test/CMakeFiles/rmq-test_int-vec.1000000.64.i.dir/clean:
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test && $(CMAKE_COMMAND) -P CMakeFiles/rmq-test_int-vec.1000000.64.i.dir/cmake_clean.cmake
.PHONY : sdsl-lite/test/CMakeFiles/rmq-test_int-vec.1000000.64.i.dir/clean

sdsl-lite/test/CMakeFiles/rmq-test_int-vec.1000000.64.i.dir/depend:
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test/CMakeFiles/rmq-test_int-vec.1000000.64.i.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sdsl-lite/test/CMakeFiles/rmq-test_int-vec.1000000.64.i.dir/depend

