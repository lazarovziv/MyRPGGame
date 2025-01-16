cmake_minimum_required(VERSION 3.28)

# macOS configurations
if (APPLE)
    set(CMAKE_CXX_COMPILER /usr/bin/clang++)

    # architecture of the cpu
    set(CMAKE_OSX_ARCHITECTURE arm64)
    set(CMAKE_PREFIX_PATH /opt/homebrew)

    # profiler
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error")
    # for ignoring the compiler error that fails the compilation
    add_compile_options(
            "-Wno-error=unused-command-line-argument"
            "-fno-omit-frame-pointer"
            "-mno-omit-leaf-frame-pointer"
    )
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif ()

# using C++ 17
set(CMAKE_CXX_STANDARD 17)

# set(CMAKE_BUILD_TYPE Release)
project(ECS)

file(GLOB SOURCE_FILES
        "*.cpp"
        "*.hpp"
        "*.h"
)

add_executable(ecs ${SOURCE_FILES})

# setting executables and libraries path to build/bin and build/lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(entt/)

set(ENTT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/ecs/entt)
target_include_directories(ecs PRIVATE ${ENTT_INCLUDE_DIR})
target_include_directories(ecs PRIVATE .)
target_link_libraries(ecs entt)

add_subdirectory(graphics)
target_include_directories(ecs PRIVATE graphics/)
target_link_libraries(ecs graphics)

# use C++ 17 compilation features - can be good for templates, overriding constructors, etc.
target_compile_features(ecs PRIVATE cxx_std_17)
set_property(TARGET ecs PROPERTY COMPILE_WARNING_AS_ERROR OFF)
