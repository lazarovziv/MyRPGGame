#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstring>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

namespace vk {

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = false;
#endif

class Device {
  private:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkInstance &instance;
    VkSurfaceKHR &surface;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    void pickPhysicalDevice();
    void createLogicalDevice();

    bool checkDeviceExtensionSupport(VkPhysicalDevice physDevice);
    bool checkValidationLayerSupport();

    bool isDeviceSuitable(VkPhysicalDevice physDevice);
    int rateDeviceSuitability(VkPhysicalDevice physDevice);

  public:
    Device(VkInstance &instance, VkSurfaceKHR &surface);
    ~Device();

    VkDevice getDevice() { return device; }
    VkPhysicalDevice getPhysicalDevice() { return physicalDevice; }
    VkQueue getGraphicsQueue() { return graphicsQueue; }
    VkQueue getPresentQueue() { return presentQueue; }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physDevice);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physDevice);
};
} // namespace vk