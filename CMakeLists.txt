cmake_minimum_required(VERSION 3.14)
project(MyRPGGame)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES MyRPGGame/src/main.cpp MyRPGGame/src/Circle.cpp
        MyRPGGame/src/Game.cpp MyRPGGame/src/GameEntity.cpp MyRPGGame/src/GameEntityBattle.cpp
        MyRPGGame/src/GameEntityMovement.cpp MyRPGGame/src/GameMap.cpp MyRPGGame/src/Heap.cpp
        MyRPGGame/src/LandscapeEntity.cpp MyRPGGame/src/MovementSubject.cpp MyRPGGame/src/NPCEnemy.cpp
        MyRPGGame/src/Player.cpp MyRPGGame/src/Point.cpp MyRPGGame/src/Projectile.cpp MyRPGGame/src/TextureLoader.cpp
        MyRPGGame/src/Weapon.cpp)

# UNIX, WIN32, WINRT, CYGWIN, APPLE are environment variables as flags set by default system
if(UNIX)
    message("This is a ${CMAKE_SYSTEM_NAME} system")
    # Darwin is Apple's system name
    if(${CMAKE_SYSTEM_NAME} MATCHES Darwin)
        message("This is a ${CMAKE_SYSTEM_NAME} system")
        set(CMAKE_OSX_ARCHITECTURES "x86_64")
    endif()
    add_executable(main.out ${SOURCE_FILES})
elseif(WIN32)
    message("This is a Windows System")
    add_executable(main.exe ${SOURCE_FILES})
endif()


find_package(SFML 2.5.1 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(main.out sfml-system sfml-window sfml-graphics sfml-audio sfml-network)