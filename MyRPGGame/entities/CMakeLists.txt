add_library(entities src/GameEntity.cpp src/LandscapeEntity.cpp src/NPCEnemy.cpp src/Player.cpp)
target_include_directories(entities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(entities sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network)
