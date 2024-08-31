#include "ShadersHelper.hpp"

namespace vk::shaders {
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

void UniformBufferObject::update(void *data, float width, float height) {
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    this->model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    this->view = glm::lookAt(glm::vec3(1.5f, 1.5f, 1.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    this->proj = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 10.0f);
    // glm was developed for opengl which sets the y coordinates upside down
    this->proj[1][1] *= -1;

    memcpy(data, this, sizeof(UniformBufferObject));
}

std::vector<char> readFile(const std::string &fileName) {
    // reading from the end of the file to determine the size of the file
    std::ifstream file(fileName, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
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
    shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    VkResult result = vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module!");
    }
    return shaderModule;
}
} // namespace vk::shaders
