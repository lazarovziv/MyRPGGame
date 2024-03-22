add_library(misc src/Heap.cpp src/Queue.cpp src/ScriptExecutor.cpp)
target_include_directories(misc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})