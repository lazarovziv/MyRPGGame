#cmake_minimum_required(VERSION 3.22)
#project(MyRPGGame)
#
#set(CMAKE_CXX_STANDARD 11)
#
#set(CMAKE_REQUIRED_LINK_OPTIONS "-arch;x86_64")
#
#include_directories(/usr/local/include)
#
#find_package(SFML 2.5.1 COMPONENTS system window graphics network audio REQUIRED)
#include_directories(${SFML_INCLUDE_DIRS})
#target_link_libraries(MyRPGGame sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
#
##include_directories(MyRPGGame /opt/homebrew/Cellar/sfml/2.5.1_1/include)
##link_directories(MyRPGGame /opt/homebrew/Cellar/sfml/2.5.1_1/lib)
#add_executable(main MyRPGGame/src/main.cpp)

cmake_minimum_required(VERSION 3.14)
project(MyRPGGame)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_OSX_ARCHITECTURES "x86_64")

set(SOURCE_FILES MyRPGGame/src/main.cpp MyRPGGame/src/Circle.cpp
        MyRPGGame/src/Game.cpp MyRPGGame/src/GameEntity.cpp MyRPGGame/src/GameEntityBattle.cpp
        MyRPGGame/src/GameEntityMovement.cpp MyRPGGame/src/GameMap.cpp MyRPGGame/src/Heap.cpp
        MyRPGGame/src/LandscapeEntity.cpp MyRPGGame/src/MovementSubject.cpp MyRPGGame/src/NPCEnemy.cpp
        MyRPGGame/src/Player.cpp MyRPGGame/src/Point.cpp MyRPGGame/src/Projectile.cpp MyRPGGame/src/TextureLoader.cpp
        MyRPGGame/src/Weapon.cpp)

add_executable(main.out ${SOURCE_FILES})
#include_directories(/usr/local/include)

find_package(SFML 2.5.1 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(main.out sfml-system sfml-window sfml-graphics sfml-audio sfml-network)