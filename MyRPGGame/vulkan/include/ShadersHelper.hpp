#include <fstream>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace shaders {

    std::vector<char> read_file(const std::string &file_name);

    VkShaderModule create_shader_module(const VkDevice &device, const std::vector<char> &code);
}