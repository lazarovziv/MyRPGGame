add_library(common src/Graph.cpp src/Subject.cpp src/Vector.cpp)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})