add_library(maps src/GameMap.cpp)
target_include_directories(maps PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(maps sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network)
