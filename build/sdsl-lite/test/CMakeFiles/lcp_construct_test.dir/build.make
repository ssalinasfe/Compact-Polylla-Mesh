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

# Include any dependencies generated for this target.
include sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/depend.make

# Include the progress variables for this target.
include sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/progress.make

# Include the compile flags for this target's objects.
include sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/flags.make

sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/lcp_construct_test.cpp.o: sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/flags.make
sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/lcp_construct_test.cpp.o: ../sdsl-lite/test/lcp_construct_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/lcp_construct_test.cpp.o"
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lcp_construct_test.dir/lcp_construct_test.cpp.o -c /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/lcp_construct_test.cpp

sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/lcp_construct_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lcp_construct_test.dir/lcp_construct_test.cpp.i"
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/lcp_construct_test.cpp > CMakeFiles/lcp_construct_test.dir/lcp_construct_test.cpp.i

sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/lcp_construct_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lcp_construct_test.dir/lcp_construct_test.cpp.s"
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/lcp_construct_test.cpp -o CMakeFiles/lcp_construct_test.dir/lcp_construct_test.cpp.s

# Object files for target lcp_construct_test
lcp_construct_test_OBJECTS = \
"CMakeFiles/lcp_construct_test.dir/lcp_construct_test.cpp.o"

# External object files for target lcp_construct_test
lcp_construct_test_EXTERNAL_OBJECTS =

sdsl-lite/test/lcp_construct_test: sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/lcp_construct_test.cpp.o
sdsl-lite/test/lcp_construct_test: sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/build.make
sdsl-lite/test/lcp_construct_test: sdsl-lite/lib/libsdsl.a
sdsl-lite/test/lcp_construct_test: sdsl-lite/external/googletest/googletest/libgtest.a
sdsl-lite/test/lcp_construct_test: sdsl-lite/external/libdivsufsort/lib/libdivsufsort.a
sdsl-lite/test/lcp_construct_test: sdsl-lite/external/libdivsufsort/lib/libdivsufsort64.a
sdsl-lite/test/lcp_construct_test: sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable lcp_construct_test"
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lcp_construct_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/build: sdsl-lite/test/lcp_construct_test

.PHONY : sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/build

sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/clean:
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test && $(CMAKE_COMMAND) -P CMakeFiles/lcp_construct_test.dir/cmake_clean.cmake
.PHONY : sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/clean

sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/depend:
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sdsl-lite/test/CMakeFiles/lcp_construct_test.dir/depend

