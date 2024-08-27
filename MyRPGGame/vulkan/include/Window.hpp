// #define GLFW_INCLUDE_VULKAN
// #include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#define VK_EXT_DEBUG_UTILS_EXTENSION_NAME "VK_EXT_debug_utils"

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <vector>
#include <map>
#include <set>

#include <memory>
#include <limits>
#include <algorithm> // for std::clamp

#include <string.h>
#include <optional>

#include "ShadersHelper.hpp"

namespace vk {
    
    const uint32_t WIDTH = 1920;
    const uint32_t HEIGHT = 1080;

    const int MAX_FRAMES_IN_FLIGHT = 2;

    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    const std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
        
        bool is_complete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = false;
    #endif

    class Window {
    private:
        GLFWwindow *window_;
        VkInstance instance;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice device;

        VkSwapchainKHR swapChain;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;
        std::vector<VkFramebuffer> swapChainFrameBuffers;

        // connects between the devices and the GLFW window
        VkSurfaceKHR surface;

        // interacting with queues
        VkQueue graphicsQueue; // automatically cleaned up when device is destroyed
        VkQueue presentQueue;

        VkPipelineLayout pipelineLayout;
        VkRenderPass renderPass;

        VkPipeline graphicsPipeline;

        // used for drawing
        VkCommandPool commandPool;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentFrame = 0;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;

        bool frameBufferResized = false;

        const std::vector<shaders::Vertex> vertices = {
            { { 0.0f, -0.75f }, { 1.0f, 1.0f, 1.0f } },
            { { 0.8f, 0.25f }, { 0.0f, 1.0f, 0.0f } },
            { { -0.1f, 0.9f }, { 0.0f, 0.0f, 1.0f } },
        };

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

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
        void createCommandBuffers();

        void createSyncObjects();

        void recreateSwapChain();
        void cleanupSwapChain();

        void recordCommandBuffer(const VkCommandBuffer &commandBuffer, const uint32_t imageIndex);

        void pickPhysicalDevice();
        bool isDeviceSuitable(const VkPhysicalDevice &device);
        int rateDeviceSuitability(const VkPhysicalDevice &device);

        void createLogicalDevice();

        QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice &device);
        SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice &device);

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &presentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        bool checkDeviceExtensionSupport(const VkPhysicalDevice &device);
        // validation
        bool checkValidationLayerSupport();
        std::vector<const char*> getRequiredExtensions();

        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        void drawFrame();

    public:
        Window() = default;
        ~Window() = default;

        void run();
    };
}