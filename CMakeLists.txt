cmake_minimum_required(VERSION 3.28)

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
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

# using C++ 17
set(CMAKE_CXX_STANDARD 17)

project(MyRPGGame)
# setting executables and libraries path to build/bin and build/lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(MyRPGGame/core)
add_subdirectory(MyRPGGame/tests)

set(CORE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/MyRPGGame/core/include)
target_include_directories(tests PUBLIC ${CORE_INCLUDE_DIR})

# use C++ 17 compilation features - can be good for templates, overriding constructors, etc.
target_compile_features(main PRIVATE cxx_std_17)
set_property(TARGET main PROPERTY COMPILE_WARNING_AS_ERROR OFF)
