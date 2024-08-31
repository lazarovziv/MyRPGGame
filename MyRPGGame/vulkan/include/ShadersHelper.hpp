#include <array>
#include <chrono>
#include <cstring>
#include <fstream>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vk::shaders {

struct Vertex {
    glm::vec2 position;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescriptions();
    static std::array<VkVertexInputAttributeDescription, 2> getAttributesDescriptions();
};

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;

    void update(void *data, float width, float height);
};

std::vector<char> readFile(const std::string &fileName);

VkShaderModule createShaderModule(const VkDevice &device, const std::vector<char> &code);
} // namespace vk::shaders
