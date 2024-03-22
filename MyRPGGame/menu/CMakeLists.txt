add_library(menu src/CharacterCreationMenu.cpp src/MainMenu.cpp src/Menu.cpp)
target_include_directories(menu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(menu sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network)
