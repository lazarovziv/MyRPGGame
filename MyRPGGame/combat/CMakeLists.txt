add_library(combat src/GameEntityBattle.cpp)
target_include_directories(combat PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(combat sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network)
