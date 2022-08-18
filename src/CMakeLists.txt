#file(GLOB SOURCE_FILES *.cpp)
#file(GLOB HEADER_FILES *.hpp)

add_library(polyllafiles STATIC  triangulation.hpp HalfEdge_AoS.hpp polylla.hpp)
#add_library(voidfiles STATIC compressHalfEdge.hpp polylla.hpp)
#add_library(voidfiles STATIC compressHalfEdgeJose.hpp polylla.hpp)

target_link_libraries(polyllafiles PUBLIC sdsl )