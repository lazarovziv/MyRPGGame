add_library(animation src/AnimationManager.cpp src/TerrainGenerator.cpp src/TextureLoader.cpp)
target_include_directories(animation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(animation sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network)

# add_subdirectory(tests)
# TODO: in tests subdirectory
# add_executable(animation_test tests/test_file.cpp)
# target_link_libraries(animation_test animation)
