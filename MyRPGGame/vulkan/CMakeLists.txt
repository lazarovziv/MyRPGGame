cmake_minimum_required(VERSION 3.28)

# using C++ 17
set(CMAKE_CXX_STANDARD 17)

project(VulkanMyRPGGame)
# setting executables and libraries path to build/bin and build/lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# getting the GLFW files
include(FetchContent)
FetchContent_Declare(GLFW
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.4)
FetchContent_MakeAvailable(GLFW)

# getting the glad files
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
)

FetchContent_MakeAvailable(glad)

# getting the glm files
FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 1.0.1)
FetchContent_MakeAvailable(glm)

set(VULKAN_SOURCE_FILES 
        src/Window.cpp
        src/ShadersHelper.cpp
)

add_library(vulkan_lib ${VULKAN_SOURCE_FILES})

target_include_directories(vulkan_lib PUBLIC include/)

# finding and linking Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(vulkan_lib Vulkan::Vulkan)

target_link_libraries(vulkan_lib glfw glad glm::glm)

find_package(OpenGL REQUIRED)
target_link_libraries(vulkan_lib OpenGL::GL)

# additional configurations for windows to use the right .dll file in the linking process
if (WIN32)

    add_custom_command(
            TARGET main_lib
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:main_lib>
            VERBATIM)

endif()

# creating the executable
add_executable(vulkan src/main.cpp)
target_include_directories(vulkan PUBLIC include/)
target_link_libraries(vulkan vulkan_lib)

# use C++ 17 compilation features - can be good for templates, overriding constructors, etc.
target_compile_features(vulkan PRIVATE cxx_std_17)
set_property(TARGET vulkan PROPERTY COMPILE_WARNING_AS_ERROR OFF)

# finding the shaders compiler
find_program(GLSLC_EXECUTABLE glslc)
if (NOT GLSLC_EXECUTABLE)
    message(FATAL_ERROR "glslc not found. Please install the Vulkan SDK.")
endif()

# this seems to do it all, but seems overly complex.
# -- add_custom_command: OUTPUT variety to create xxx.spv file dependent on the shader.xxx files
# -- add_custom_target: new target dependent on xxx.spv files
# -- add_dependencies: to associate my_shaders target with main ${VULCAN_PROJECT_NAME} target
function(compile_shader NAME)
    # create a variable of all compiled shader file names, for add_custom_target
    set(SPV_FILES ${SPV_FILES} ${CMAKE_SOURCE_DIR}/shaders/${NAME}.spv PARENT_SCOPE)

    # Build a command to compile each xxx.spv from shader.xxx source
    add_custom_command(
        OUTPUT ${CMAKE_SOURCE_DIR}/shaders/${NAME}.spv
        COMMAND ${VULKAN_SDK_PATH}/bin/glslc shader.${NAME} -o ${NAME}.spv
        MAIN_DEPENDENCY shaders/shader.${NAME}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/shaders
        COMMENT "Compiling shader.${NAME} to ${NAME}.spv"
    )
endfunction()

# call these before add_custom_target, to build the SPV_FILES variable
compile_shader(vert)
compile_shader(frag)
add_custom_target(shaders DEPENDS ${SPV_FILES})
add_dependencies(vulkan shaders)
