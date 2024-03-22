add_library(repositories_entities src/EnemyRepository.cpp src/GameEntityRepository.cpp src/PlayerRepository.cpp)
target_include_directories(repositories_entities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(repositories_entities sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network)
