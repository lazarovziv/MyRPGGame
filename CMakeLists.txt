cmake_minimum_required(VERSION 3.14)
# TODO: change file to be used in the docker container
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

if (APPLE)
    set(CMAKE_CXX_COMPILER /usr/bin/g++)
    set(CMAKE_OSX_ARCHITECTURE arm64)
    set(FULL_PATH_TO_CPP_MYSQL_CONNECTOR /usr/local/mysql-connector-c++-8.0.32)
    set(CMAKE_PREFIX_PATH /opt/homebrew)
elseif (WIN32)
    # set windows compiler
    set(SFML_DIR C:/SFML/lib/cmake/SFML)
    set(OpenCV_DIR C:/opencv/build/x64/vc16/lib)
else()
    set(CMAKE_CXX_COMPILER /usr/bin/clang++)
    set(CMAKE_C_COMPILER  /usr/bin/clang)

    set(SFML_DIR /workspace/SFML-2.6.0)
    set(OpenCV_DIR /workspace/opencv-4.8.0/build)
endif()
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

find_package(SFML 2.6.0 COMPONENTS system window graphics network audio REQUIRED)
find_package(OpenCV COMPONENTS core highgui REQUIRED)

if (APPLE)
    include_directories(${SFML_INCLUDE_DIRS}/include)
    include_directories(${FULL_PATH_TO_CPP_MYSQL_CONNECTOR}/include)

    link_directories(${FULL_PATH_TO_CPP_MYSQL_CONNECTOR}/lib64)
else()
    # include_directories(${SFML_INCLUDE_DIR})
   include_directories(${SFML_DIR}/include)
endif()

include_directories(${OpenCV_INCLUDE_DIRS})

# creating executables
add_executable(main ${SOURCE_FILES})
target_link_libraries(main sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network)
target_link_libraries(main ${OpenCV_LIBS})

target_compile_features(main PRIVATE cxx_range_for)