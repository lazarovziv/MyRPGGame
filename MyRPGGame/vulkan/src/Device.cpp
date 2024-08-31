#include "Device.hpp"

namespace vk {

Device::Device(VkInstance &instance, VkSurfaceKHR &surface) : instance{instance}, surface{surface} {
    pickPhysicalDevice();
    createLogicalDevice();
}

Device::~Device() { vkDestroyDevice(device, nullptr); }

void Device::pickPhysicalDevice() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    // if no devices found
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    // creating a vector to store all gpus
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    // ordered map (by key) to be able to get highest scoring device
    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto &device : devices) {
        if (!isDeviceSuitable(device))
            continue;

        int score = rateDeviceSuitability(device);
        candidates.insert({score, device});
        // candidates.insert(std::make_pair(score, device));
    }

    // getting the highest scoring gpu
    if (candidates.rbegin()->first > 0) {
        physicalDevice = candidates.rbegin()->second;
    } else
        throw std::runtime_error("Failed to find a suitable GPU!");
}

void Device::createLogicalDevice() {
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    // since there's a possibility for a device to not support presentation, we'll create a device for each queue
    // family
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    // equal priority for all queue families
    float queuePriority = 1.0f;

    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queue_create_infos.push_back(queueCreateInfo);
    }

    // getting optional features for the gpu we're using
    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    // add pointers to the queue creation info and for the features info
    deviceCreateInfo.pQueueCreateInfos = queue_create_infos.data();
    deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    // TODO: specify extensions and validation layers for this device
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (vk::enableValidationLayers) {
        deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(vk::validationLayers.size());
        deviceCreateInfo.ppEnabledLayerNames = vk::validationLayers.data();
    } else {
        deviceCreateInfo.enabledLayerCount = 0;
    }

    // create the logical device
    VkResult result = vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create a logical device!");
    }
    // logical device created

    // retrieving queue handles for each queue family, but we're using a single queue so setting index to 0
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

bool Device::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    // filling required extensions with device extensions defined in Window.hpp
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    // removing all extensions from required extensions
    for (const auto &extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }
    // if the list is not empty, there's a required extensions that's not available
    return requiredExtensions.empty();
}

SwapChainSupportDetails Device::querySwapChainSupport(VkPhysicalDevice physDevice) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physDevice, surface, &details.capabilities);

    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        // changing the initial size of the formats vector to match the enumeration findings
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physDevice, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        // doing the same for the present modes vector
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physDevice, surface, &presentModeCount,
                                                  details.presentModes.data());
    }

    return details;
}

bool Device::isDeviceSuitable(VkPhysicalDevice physDevice) {
    QueueFamilyIndices indices = findQueueFamilies(physDevice);

    bool extensionsSupported = checkDeviceExtensionSupport(physDevice);
    // checking swap chain support AFTER checking extension is available
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physDevice);
        // adequate only if both vectors are NOT empty
        swapChainAdequate = !(swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty());
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

int Device::rateDeviceSuitability(VkPhysicalDevice physDevice) {
    // enumerating the device
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(physDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures(physDevice, &deviceFeatures);

    int score = 0;
    // dedicated gpu
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }
    // max size of textures
    score += deviceProperties.limits.maxImageDimension2D;

    // must have a geometry shader
    if (!deviceFeatures.geometryShader)
        return 0;

    return score;
}

QueueFamilyIndices Device::findQueueFamilies(VkPhysicalDevice physDevice) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &queueFamilyCount, nullptr);
    // storing all data about queue families for given device, including the type of operations that are supported
    // and the number of queues that can be created based on the family
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto &queueFamily : queueFamilies) {
        VkBool32 presentSupport = false;
        // presentation support implies swap chain extension support
        vkGetPhysicalDeviceSurfaceSupportKHR(physDevice, i, surface, &presentSupport);
        if (presentSupport)
            indices.presentFamily = i;

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            indices.graphicsFamily = i;

        // if found suitable device with relevant queue families and number of queues
        if (indices.isComplete())
            break;

        ++i;
    }

    return indices;
}
} // namespace vk
