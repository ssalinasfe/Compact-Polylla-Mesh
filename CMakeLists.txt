cmake_minimum_required(VERSION 3.10)

# set the project name
project(Delfin)

# add the executable
add_executable(Delfin main.cpp)

add_subdirectory(sdsl-lite)
target_link_libraries(Delfin PUBLIC sdsl)