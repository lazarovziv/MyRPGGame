#include "ShadersHelper.hpp"

namespace shaders {
    std::vector<char> read_file(const std::string &file_name) {
        // reading from the end of the file to determine the size of the file
        std::ifstream file(file_name, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file!");
        }

        size_t file_size = (size_t) file.tellg();
        std::vector<char> buffer(file_size);
        // go to the start
        file.seekg(0);
        file.read(buffer.data(), file_size);

        file.close();

        return buffer;
    }

    VkShaderModule create_shader_module(const VkDevice &device, const std::vector<char> &code) {
        VkShaderModule shader_module;

        VkShaderModuleCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        create_info.codeSize = code.size();
        create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkResult result = vkCreateShaderModule(device, &create_info, nullptr, &shader_module);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create shader module!");
        }
        return shader_module;
    }
}