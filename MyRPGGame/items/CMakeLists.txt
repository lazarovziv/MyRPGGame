add_library(items src/Weapon.cpp)
target_include_directories(items PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(items sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network)

