cmake_minimum_required(VERSION 3.14)

# making the sfml libraries to be built for static linking
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# getting the SFML files
include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.0)
FetchContent_MakeAvailable(SFML)

set(SOURCE_FILES src/Circle.cpp src/MenuRepository.cpp
        src/Menu.cpp src/TerrainGenerator.cpp
        # physics
        src/Vector.cpp src/Collision.cpp src/RigidBody.cpp
        src/RigidBodyForceRegistry.cpp src/RigidBodyForceRegistry.cpp
        src/RigidBodyGravity.cpp src/RigidBodyDrag.cpp
        # physics
        src/ScriptExecutor.cpp src/AnimationManager.cpp
        src/Game.cpp src/GameEntity.cpp src/GameEntityBattle.cpp
        src/GameEntityMovement.cpp src/GameMap.cpp src/Heap.cpp
        src/LandscapeEntity.cpp src/Subject.cpp
        src/NPCEnemy.cpp src/Player.cpp src/Point.cpp
        src/Projectile.cpp src/TextureLoader.cpp src/Weapon.cpp
        src/Graph.cpp
        src/GameEntityRepository.cpp src/PlayerRepository.cpp src/EnemyRepository.cpp
        #generate_img_cpp.cpp
)

# creating the library
add_library(main_lib ${SOURCE_FILES})

# linking sfml
target_link_libraries(main_lib sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network)

# finding and linking OpenCV
#find_package(OpenCV REQUIRED)
#target_link_libraries(main ${OpenCV_LIBS})

# additional configurations for windows to use the right .dll file in the linking process
if (WIN32)

    add_custom_command(
            TARGET main
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main_lib>
            VERBATIM)

endif()

# creating the executable
add_executable(main src/main.cpp)
target_link_libraries(main main_lib)
