cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_COMPILER /usr/bin/gcc)
#set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_CXX_STANDARD 17)

project(MyRPGGame)

#set(FULL_PATH_TO_CPP_MYSQL_CONNECTOR /usr/local/mysql-connector-c++-8.0.32)
#set(PYTHON_INCLUDE_DIR /usr/local/include/python3.9)

set(SOURCE_FILES MyRPGGame/src/main.cpp MyRPGGame/src/Circle.cpp MyRPGGame/src/Menu.cpp
        MyRPGGame/src/MainMenu.cpp MyRPGGame/src/CharacterCreationMenu.cpp MyRPGGame/src/PythonScriptExecutor.cpp
        MyRPGGame/src/Game.cpp MyRPGGame/src/GameEntity.cpp MyRPGGame/src/GameEntityBattle.cpp
        MyRPGGame/src/GameEntityMovement.cpp MyRPGGame/src/GameMap.cpp MyRPGGame/src/Heap.cpp
        MyRPGGame/src/LandscapeEntity.cpp MyRPGGame/src/Subject.cpp 
        MyRPGGame/src/NPCEnemy.cpp MyRPGGame/src/Player.cpp MyRPGGame/src/Point.cpp
        MyRPGGame/src/Projectile.cpp MyRPGGame/src/TextureLoader.cpp MyRPGGame/src/Weapon.cpp
        MyRPGGame/src/Graph.cpp MyRPGGame/src/PlayerRepository.cpp MyRPGGame/src/EnemyRepository.cpp)

find_package(SFML 2.6.0 COMPONENTS system window graphics network audio REQUIRED)
#find_package(Python REQUIRED)
#find_package(ImageMagick COMPONENTS Magick++)
find_package(OpenCV REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
#include_directories(${FULL_PATH_TO_CPP_MYSQL_CONNECTOR}/include)
#include_directories(${IMAGE_MAGICK_INCLUDE_DIR})
#include_directories(${ImageMagick_INCLUDE_DIRS})
#include_directories(${PYTHON_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})

#link_directories(${FULL_PATH_TO_CPP_MYSQL_CONNECTOR}/lib64)

add_executable(main.out ${SOURCE_FILES})

target_link_libraries(main.out sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network)
#target_link_libraries(main.out ${ImageMagick_LIBRARIES})
#target_link_libraries(main.out ${PYTHON_LIBRARIES})
target_link_libraries(main.out ${OpenCV_LIBS})
#target_link_directories(main.out ${IMAGEMAGICK_LIBS})

if(LINUX)
    link_directories("${PROJECT_SOURCE_DIR}/platforms/linux")
    add_executable(main.out ${SOURCE_FILES})
    find_package(SFML 2.6.0 COMPONENTS system window graphics network audio REQUIRED)
    include_directories(${SFML_INCLUDE_DIRS})
#    include_directories(${FULL_PATH_TO_CPP_MYSQL_CONNECTOR}/include)
#    link_directories(${FULL_PATH_TO_CPP_MYSQL_CONNECTOR}/lib64)
    target_link_libraries(main.out sfml-system
            sfml-window
            sfml-graphics
            sfml-audio
            sfml-network)
    target_compile_features(main.out PRIVATE cxx_range_for)
elseif(MACOS)
    link_directories("${PROJECT_SOURCE_DIR}/platforms/macos")
#    set(CMAKE_OSX_ARCHITECTURES "x86_64")
    add_executable(main.out ${SOURCE_FILES})
    find_package(SFML 2.6.0 COMPONENTS system window graphics network audio REQUIRED)
    include_directories(${SFML_INCLUDE_DIRS})
#    include_directories(${FULL_PATH_TO_CPP_MYSQL_CONNECTOR}/include)
#    link_directories(${FULL_PATH_TO_CPP_MYSQL_CONNECTOR}/lib64)
    target_link_libraries(main.out sfml-system
            sfml-window
            sfml-graphics
            sfml-audio
            sfml-network)
    target_compile_features(main.out PRIVATE cxx_range_for)
elseif(WINDOWS)
    link_directories("${PROJECT_SOURCE_DIR}/platforms/windows")
    add_executable(main.exe ${SOURCE_FILES})
    find_package(SFML 2.6.0 COMPONENTS system window graphics network audio REQUIRED)
    include_directories(${SFML_INCLUDE_DIRS})
#    include_directories(${FULL_PATH_TO_CPP_MYSQL_CONNECTOR}/include)
#    link_directories(${FULL_PATH_TO_CPP_MYSQL_CONNECTOR}/lib64)
    target_link_libraries(main.exe sfml-system
            sfml-window
            sfml-graphics
            sfml-audio
            sfml-network)
    target_compile_features(main.exe PRIVATE cxx_range_for)
endif()