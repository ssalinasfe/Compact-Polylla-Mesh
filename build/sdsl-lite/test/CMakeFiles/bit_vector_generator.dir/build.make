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
include sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/depend.make

# Include the progress variables for this target.
include sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/progress.make

# Include the compile flags for this target's objects.
include sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/flags.make

sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/bit_vector_generator.cpp.o: sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/flags.make
sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/bit_vector_generator.cpp.o: ../sdsl-lite/test/bit_vector_generator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/bit_vector_generator.cpp.o"
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bit_vector_generator.dir/bit_vector_generator.cpp.o -c /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/bit_vector_generator.cpp

sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/bit_vector_generator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bit_vector_generator.dir/bit_vector_generator.cpp.i"
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/bit_vector_generator.cpp > CMakeFiles/bit_vector_generator.dir/bit_vector_generator.cpp.i

sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/bit_vector_generator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bit_vector_generator.dir/bit_vector_generator.cpp.s"
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test && /bin/x86_64-linux-gnu-g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test/bit_vector_generator.cpp -o CMakeFiles/bit_vector_generator.dir/bit_vector_generator.cpp.s

# Object files for target bit_vector_generator
bit_vector_generator_OBJECTS = \
"CMakeFiles/bit_vector_generator.dir/bit_vector_generator.cpp.o"

# External object files for target bit_vector_generator
bit_vector_generator_EXTERNAL_OBJECTS =

sdsl-lite/test/bit_vector_generator: sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/bit_vector_generator.cpp.o
sdsl-lite/test/bit_vector_generator: sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/build.make
sdsl-lite/test/bit_vector_generator: sdsl-lite/lib/libsdsl.a
sdsl-lite/test/bit_vector_generator: sdsl-lite/external/googletest/googletest/libgtest.a
sdsl-lite/test/bit_vector_generator: sdsl-lite/external/libdivsufsort/lib/libdivsufsort.a
sdsl-lite/test/bit_vector_generator: sdsl-lite/external/libdivsufsort/lib/libdivsufsort64.a
sdsl-lite/test/bit_vector_generator: sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bit_vector_generator"
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bit_vector_generator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/build: sdsl-lite/test/bit_vector_generator

.PHONY : sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/build

sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/clean:
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test && $(CMAKE_COMMAND) -P CMakeFiles/bit_vector_generator.dir/cmake_clean.cmake
.PHONY : sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/clean

sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/depend:
	cd /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/sdsl-lite/test /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test /home/cuyiguaton/Dropbox/Doctorado/comprimidas/programa/build/sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sdsl-lite/test/CMakeFiles/bit_vector_generator.dir/depend

