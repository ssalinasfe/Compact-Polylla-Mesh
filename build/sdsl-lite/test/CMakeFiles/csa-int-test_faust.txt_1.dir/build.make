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

# Utility rule file for csa-int-test_faust.txt_1.

# Include the progress variables for this target.
include sdsl-lite/test/CMakeFiles/csa-int-test_faust.txt_1.dir/progress.make

sdsl-lite/test/CMakeFiles/csa-int-test_faust.txt_1: ../sdsl-lite/test/test_cases/faust.txt
sdsl-lite/test/CMakeFiles/csa-int-test_faust.txt_1: ../sdsl-lite/test/tmp
sdsl-lite/test/CMakeFiles/csa-int-test_faust.txt_1: sdsl-lite/test/csa_int_test
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Execute csa-int-test on faust.txt."
	/home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test/csa_int_test /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/test_cases/faust.txt 1 /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/tmp/csa_int_test__1 /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/tmp

../sdsl-lite/test/test_cases/faust.txt:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Checking or downloading test case faust.txt."
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test && /usr/bin/cmake -Ddownload_url=http://algo2.iti.kit.edu/gog/sdsl/faust.txt.tar.gz -Dtest_case_dir=/home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/test_cases -P /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/download.cmake

csa-int-test_faust.txt_1: sdsl-lite/test/CMakeFiles/csa-int-test_faust.txt_1
csa-int-test_faust.txt_1: ../sdsl-lite/test/test_cases/faust.txt
csa-int-test_faust.txt_1: sdsl-lite/test/CMakeFiles/csa-int-test_faust.txt_1.dir/build.make

.PHONY : csa-int-test_faust.txt_1

# Rule to build all files generated by this target.
sdsl-lite/test/CMakeFiles/csa-int-test_faust.txt_1.dir/build: csa-int-test_faust.txt_1

.PHONY : sdsl-lite/test/CMakeFiles/csa-int-test_faust.txt_1.dir/build

sdsl-lite/test/CMakeFiles/csa-int-test_faust.txt_1.dir/clean:
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test && $(CMAKE_COMMAND) -P CMakeFiles/csa-int-test_faust.txt_1.dir/cmake_clean.cmake
.PHONY : sdsl-lite/test/CMakeFiles/csa-int-test_faust.txt_1.dir/clean

sdsl-lite/test/CMakeFiles/csa-int-test_faust.txt_1.dir/depend:
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test/CMakeFiles/csa-int-test_faust.txt_1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sdsl-lite/test/CMakeFiles/csa-int-test_faust.txt_1.dir/depend

