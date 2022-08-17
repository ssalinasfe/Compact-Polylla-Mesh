#file(GLOB SOURCE_FILES *.cpp)
#file(GLOB HEADER_FILES *.hpp)

add_library(voidfiles STATIC HalfEdge_AoS.hpp CompactPolylla.hpp CompactHalfEdge.hpp Polylla_Aos.hpp CompactPolylla_no_matrix.hpp)
#add_library(voidfiles STATIC compressHalfEdge.hpp polylla.hpp)
#add_library(voidfiles STATIC compressHalfEdgeJose.hpp polylla.hpp)

target_link_libraries(voidfiles PUBLIC sdsl)