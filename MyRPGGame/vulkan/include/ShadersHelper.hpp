#include <fstream>
#include <vector>
#include <array>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace vk {

namespace shaders {

        struct Vertex {
            glm::vec2 position;
            glm::vec3 color;

            static VkVertexInputBindingDescription getBindingDescriptions();
            static std::array<VkVertexInputAttributeDescription, 2> getAttributesDescriptions();
        };

        std::vector<char> readFile(const std::string &fileName);

        VkShaderModule createShaderModule(const VkDevice &device, const std::vector<char> &code);
    }
}
