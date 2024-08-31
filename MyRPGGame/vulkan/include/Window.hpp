#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define VK_EXT_DEBUG_UTILS_EXTENSION_NAME "VK_EXT_debug_utils"

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <iostream>
#include <algorithm> // for std::clamp
#include <limits>
#include <memory>

#include "Device.hpp"
#include "ShadersHelper.hpp"

namespace vk {

const uint32_t WIDTH = 1920;
const uint32_t HEIGHT = 1080;

const int MAX_FRAMES_IN_FLIGHT = 2;

class Window {
  private:
    GLFWwindow *window_;
    // connecting between the application and the vulkan library
    VkInstance instance;
    // connects between the devices and the GLFW window
    VkSurfaceKHR surface;

    std::unique_ptr<Device> device;

    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;

    std::vector<VkImage> swapChainImages;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFrameBuffers;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;

    VkPipeline graphicsPipeline;

    // used for drawing
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void *> uniformBuffersMapped;

    uint32_t currentFrame = 0;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    bool frameBufferResized = false;

    const std::vector<shaders::Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    };

    const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    void initWindow();

    static void frameBufferResizeCallback(GLFWwindow *window, int width, int height);
    void initVulkan();
    void mainLoop();
    void cleanup();

    void createInstance();
    void createSurface();
    void createSwapChain();
    void createFrameBuffers();

    void createImageViews();
    void createGraphicsPipeline();
    void createRenderPass();

    void createCommandPool();
    void createVertexBuffer();
    void createBuffer(VkDeviceSize &size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                      VkBuffer &buffer, VkDeviceMemory &bufferMemory);
    void createIndexBuffer();
    void copyBuffer(VkBuffer &srcBuffer, VkBuffer &dstBuffer, VkDeviceSize size);

    void createDescriptorSetLayout();
    void createUniformBuffers();
    void createDescriptorPool();
    void createDescriptorSets();

    void createCommandBuffers();

    void createSyncObjects();

    void recreateSwapChain();
    void cleanupSwapChain();

    void recordCommandBuffer(const VkCommandBuffer &commandBuffer, const uint32_t imageIndex);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &presentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    std::vector<const char *> getRequiredExtensions();

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void drawFrame();

  public:
    Window() = default;
    ~Window() = default;

    void run();
};
} // namespace vk
