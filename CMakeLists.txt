cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_COMPILER /usr/bin/g++)
#set(CMAKE_OSX_ARCHITECTURE x86_64)
set(CMAKE_CXX_STANDARD 17)

project(MyRPGGame)

set(FULL_PATH_TO_CPP_MYSQL_CONNECTOR /usr/local/mysql-connector-c++-8.0.32)

set(SOURCE_FILES MyRPGGame/src/main.cpp MyRPGGame/src/Circle.cpp MyRPGGame/src/MenuRepository.cpp
        MyRPGGame/src/Menu.cpp
        MyRPGGame/src/PythonScriptExecutor.cpp
        MyRPGGame/src/Game.cpp MyRPGGame/src/GameEntity.cpp MyRPGGame/src/GameEntityBattle.cpp
        MyRPGGame/src/GameEntityMovement.cpp MyRPGGame/src/GameMap.cpp MyRPGGame/src/Heap.cpp
        MyRPGGame/src/LandscapeEntity.cpp MyRPGGame/src/Subject.cpp 
        MyRPGGame/src/NPCEnemy.cpp MyRPGGame/src/Player.cpp MyRPGGame/src/Point.cpp
        MyRPGGame/src/Projectile.cpp MyRPGGame/src/TextureLoader.cpp MyRPGGame/src/Weapon.cpp
        MyRPGGame/src/Graph.cpp MyRPGGame/src/PlayerRepository.cpp MyRPGGame/src/EnemyRepository.cpp)

find_package(SFML 2.6.0 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS}/include)
include_directories(${FULL_PATH_TO_CPP_MYSQL_CONNECTOR}/include)

link_directories(${FULL_PATH_TO_CPP_MYSQL_CONNECTOR}/lib64)

add_executable(main.out ${SOURCE_FILES})

target_link_libraries(main.out sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network)

target_compile_features(main.out PRIVATE cxx_range_for)