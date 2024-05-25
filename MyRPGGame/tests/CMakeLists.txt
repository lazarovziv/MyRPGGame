cmake_minimum_required(VERSION 3.14)

enable_testing()

# getting the Google Test files
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)
# prevent overriding the parent project's compiler/linker settings for windows
if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif ()
#FetchContent_MakeAvailable(googletest)

# all test files go here
set(TESTS_SOURCE_FILES
        src/main.cpp
        src/GoogleTestSample.cpp
)

# adding to the tests executable
add_executable(tests ${TESTS_SOURCE_FILES})

# linking the core library to the tests executable
target_link_libraries(tests PRIVATE main_lib)

# adding googletest as submodule w/o installing it
set(INSTALL_GTEST OFF)
add_subdirectory(googletest)
# linking relevant test libraries to our tests executable
target_link_libraries(tests PRIVATE gtest gtest_main)

# register tests to the executable
include(GoogleTest)
gtest_discover_tests(tests)
