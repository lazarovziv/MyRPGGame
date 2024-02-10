cmake_minimum_required(VERSION 3.14)

# macOS configurations
if (APPLE)
    set(CMAKE_CXX_COMPILER /usr/bin/clang++)
    # architecture of the cpu
    set(CMAKE_OSX_ARCHITECTURE arm64)
    set(CMAKE_PREFIX_PATH /opt/homebrew)
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error")
    # for ignoring the compiler error that fails the compilation
    add_compile_options(
            "-Wno-error=unused-command-line-argument"
    )
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release/)
endif()
# making the sfml libraries to be built for static linking
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
# getting the SFML files
include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.0)
FetchContent_MakeAvailable(SFML)

# TODO: modify if statement to be used in a linux docker container
#if (LINUX)
#    set(CMAKE_CXX_COMPILER /usr/bin/clang++)
#    set(CMAKE_C_COMPILER  /usr/bin/clang)
#
#    set(SFML_DIR /workspace/SFML-2.6.0)
#    set(OpenCV_DIR /workspace/opencv-4.8.0/build)
#endif()

# using C++ 17
set(CMAKE_CXX_STANDARD 17)

project(MyRPGGame)

set(SOURCE_FILES MyRPGGame/src/main.cpp MyRPGGame/src/Circle.cpp MyRPGGame/src/MenuRepository.cpp
        MyRPGGame/src/Menu.cpp MyRPGGame/src/TerrainGenerator.cpp
        # physics
        MyRPGGame/src/Vector.cpp MyRPGGame/src/Collision.cpp MyRPGGame/src/RigidBody.cpp
        MyRPGGame/src/RigidBodyForceRegistry.cpp MyRPGGame/src/RigidBodyForceRegistry.cpp
        MyRPGGame/src/RigidBodyGravity.cpp MyRPGGame/src/RigidBodyDrag.cpp
        # physics
        MyRPGGame/src/ScriptExecutor.cpp MyRPGGame/src/AnimationManager.cpp
        MyRPGGame/src/Game.cpp MyRPGGame/src/GameEntity.cpp MyRPGGame/src/GameEntityBattle.cpp
        MyRPGGame/src/GameEntityMovement.cpp MyRPGGame/src/GameMap.cpp MyRPGGame/src/Heap.cpp
        MyRPGGame/src/LandscapeEntity.cpp MyRPGGame/src/Subject.cpp
        MyRPGGame/src/NPCEnemy.cpp MyRPGGame/src/Player.cpp MyRPGGame/src/Point.cpp
        MyRPGGame/src/Projectile.cpp MyRPGGame/src/TextureLoader.cpp MyRPGGame/src/Weapon.cpp
        MyRPGGame/src/Graph.cpp MyRPGGame/src/PlayerRepository.cpp MyRPGGame/src/EnemyRepository.cpp
        # test files
        # test files
)

# creating the executable
add_executable(main ${SOURCE_FILES})
target_link_libraries(main sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network)
# linking OpenCV if necessary
target_link_libraries(main ${OpenCV_LIBS})
# additional configurations for windows to use the right .dll file in the linking process
if (WIN32)
    add_custom_command(
            TARGET main
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main>
            VERBATIM)
endif()

# use C++ 17 compilation features - can be good for templates, overriding constructors, etc.
target_compile_features(main PRIVATE cxx_std_17)
set_property(TARGET main PROPERTY COMPILE_WARNING_AS_ERROR OFF)