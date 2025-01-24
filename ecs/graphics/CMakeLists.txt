set(GLFW_BUILD_X11 1)
set(GLFW_BUILD_WAYLAND 0)

# getting the GLFW files
include(FetchContent)
FetchContent_Declare(GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw.git
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
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1)
FetchContent_MakeAvailable(glm)

# making the sfml libraries to be built for static linking
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# getting the SFML files
include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.0)
FetchContent_MakeAvailable(SFML)

# compile shaders
file(GLOB SHADERS
    "shaders/*.vert" "shaders/*.frag"
)
set(SHADER_COMPILER glslc)
foreach(SHADER_SOURCE ${SHADERS})
    get_filename_component(SHADER_NAME ${SHADER_SOURCE} NAME)
    set(SPIRV_FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${SHADER_NAME}.spv)
    list(APPEND SPIRV_FILES ${SPIRV_FILE})

    add_custom_command(
        OUTPUT ${SPIRV_FILE}
        COMMAND ${SHADER_COMPILER} -o ${SPIRV_FILE} ${SHADER_SOURCE}
        DEPENDS ${SHADER_SOURCE}
        COMMENT "Compiling shader: ${SHADER_SOURCE}..."
    )
endforeach()

add_custom_target(shaders ALL DEPENDS ${SPIRV_FILES})

file(GLOB SOURCE_FILES
    "*.cpp"
    "*.hpp"
    "*.h"
)

add_library(graphics ${SOURCE_FILES})
add_dependencies(graphics shaders)

# finding and linking Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(graphics Vulkan::Vulkan glfw glad glm::glm)

find_package(OpenGL REQUIRED)
target_link_libraries(graphics OpenGL::GL)

target_link_libraries(graphics sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
        sfml-network)

if (WIN32)
    add_custom_command(
            TARGET graphics
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:graphics>
            VERBATIM)
endif()
