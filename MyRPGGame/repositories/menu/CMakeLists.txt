add_library(repositories_menu src/MenuRepository.cpp)
target_include_directories(repositories_menu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(repositories_menu sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network)
