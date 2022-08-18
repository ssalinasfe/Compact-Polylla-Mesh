cmake_minimum_required(VERSION 3.10)

project(compactPolylla)

add_subdirectory(external src)
include_directories(external src)

add_executable(compactPolylla main.cpp)

target_link_libraries(compactPolylla PUBLIC sdsl malloccountfiles  ${CMAKE_DL_LIBS})