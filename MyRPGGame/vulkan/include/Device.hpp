#pragma once

#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

#include "Validation.hpp"

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

const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

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

    bool isDeviceSuitable(VkPhysicalDevice physDevice);
    int rateDeviceSuitability(VkPhysicalDevice physDevice);

  public:
    Device(VkInstance &instance, VkSurfaceKHR &surface);
    Device(const Device &) = delete;
    Device &operator=(const Device &) = delete;
    ~Device();

    VkDevice getDevice() { return device; }
    VkPhysicalDevice getPhysicalDevice() { return physicalDevice; }
    VkQueue getGraphicsQueue() { return graphicsQueue; }
    VkQueue getPresentQueue() { return presentQueue; }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physDevice);
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physDevice);
};
} // namespace vk