#include "ShadersHelper.hpp"

namespace shaders {
    VkVertexInputBindingDescription Vertex::getBindingDescriptions() {
        VkVertexInputBindingDescription bindingDescriptions{};
        bindingDescriptions.binding = 0;
        // stride per vertex (currently each vertex has a position and color, i.e: vec2 and vec3)
        bindingDescriptions.stride = sizeof(Vertex);
        // per vertex and not per instance
        bindingDescriptions.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescriptions;
    }

    std::array<VkVertexInputAttributeDescription, 2> Vertex::getAttributesDescriptions() {
        std::array<VkVertexInputAttributeDescription, 2> bindingAttributes{};
        // Vertex.position attributes
        bindingAttributes[0].binding = 0;
        bindingAttributes[0].location = 0;
        bindingAttributes[0].format = VK_FORMAT_R32G32_SFLOAT;
        bindingAttributes[0].offset = offsetof(Vertex, position);
        // Vertex.color attributes
        bindingAttributes[1].binding = 0;
        bindingAttributes[1].location = 1;
        bindingAttributes[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        bindingAttributes[1].offset = offsetof(Vertex, color);

        return bindingAttributes;
    }

    std::vector<char> readFile(const std::string &fileName) {
        // reading from the end of the file to determine the size of the file
        std::ifstream file(fileName, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file!");
        }

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);
        // go to the start
        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    VkShaderModule createShaderModule(const VkDevice &device, const std::vector<char> &code) {
        VkShaderModule shaderModule;

        VkShaderModuleCreateInfo shaderModuleCreateInfo{};
        shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCreateInfo.codeSize = code.size();
        shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkResult result = vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create shader module!");
        }
        return shaderModule;
    }
}