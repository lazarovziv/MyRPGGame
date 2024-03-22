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

add_subdirectory(MyRPGGame/animation)
add_subdirectory(MyRPGGame/combat)
add_subdirectory(MyRPGGame/common)
add_subdirectory(MyRPGGame/entities)
add_subdirectory(MyRPGGame/items)
add_subdirectory(MyRPGGame/main)
add_subdirectory(MyRPGGame/maps)
add_subdirectory(MyRPGGame/menu)
add_subdirectory(MyRPGGame/misc)
add_subdirectory(MyRPGGame/movement)
add_subdirectory(MyRPGGame/physics)
add_subdirectory(MyRPGGame/repositories)

set(SOURCE_FILES MyRPGGame/main.cpp)

# creating the executable
add_executable(game ${SOURCE_FILES})
target_link_libraries(game sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network)
# linking OpenCV if necessary
target_link_libraries(game ${OpenCV_LIBS})

# linking all our sub libraries
target_link_libraries(game animation)
target_link_libraries(game combat)
target_link_libraries(game common)
target_link_libraries(game entities)
target_link_libraries(game items)
target_link_libraries(game main)
target_link_libraries(game maps)
target_link_libraries(game menu)
target_link_libraries(game misc)
target_link_libraries(game movement)
target_link_libraries(game physics)
target_link_libraries(game repositories)

# additional configurations for windows to use the right .dll file in the linking process
if (WIN32)
    add_custom_command(
            TARGET game
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:game>
            VERBATIM)
endif()

# use C++ 17 compilation features - can be good for templates, overriding constructors, etc.
target_compile_features(game PRIVATE cxx_std_17)
set_property(TARGET game PROPERTY COMPILE_WARNING_AS_ERROR OFF)