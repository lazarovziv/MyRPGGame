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

const uint32_t WIDTH = 1920;
const uint32_t HEIGHT = 1080;

// const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validation_layers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphics_family;
    std::optional<uint32_t> present_family;
    
    bool is_complete() {
        return graphics_family.has_value() && present_family.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> present_modes;
};

#ifdef NDEBUG
    const bool enable_validation_layers = false;
#else
    const bool enable_validation_layers = false;
#endif

class Window {
private:
    GLFWwindow *window_;
    VkInstance instance;
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkDevice device;

    VkSwapchainKHR swap_chain;
    VkFormat swap_chain_image_format;
    VkExtent2D swap_chain_extent;

    std::vector<VkImage> swap_chain_images;
    std::vector<VkImageView> swap_chain_image_views;
    std::vector<VkFramebuffer> swap_chain_frame_buffers;

    // connects between the devices and the GLFW window
    VkSurfaceKHR surface;

    // interacting with queues
    VkQueue graphics_queue; // automatically cleaned up when device is destroyed
    VkQueue present_queue;

    VkPipelineLayout pipeline_layout;
    VkRenderPass render_pass;

    VkPipeline graphics_pipeline;

    // used for drawing
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;

    VkSemaphore image_available_semaphore;
    VkSemaphore render_finished_semaphore;
    VkFence in_flight_fence;

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void init_window();
    void init_vulkan();
    void main_loop();
    void cleanup();

    void create_instance();
    void create_surface();
    void create_swap_chain();
    void create_frame_buffers();

    void create_image_views();
    void create_graphics_pipeline();
    void create_render_pass();

    void create_command_pool();
    void create_command_buffer();

    void create_sync_objects();

    void record_command_buffer(const VkCommandBuffer &command_buffer, const uint32_t image_index);

    void pick_physical_device();
    bool is_device_suitable(const VkPhysicalDevice &device);
    int rate_device_suitability(const VkPhysicalDevice &device);

    void create_logical_device();

    QueueFamilyIndices find_queue_families(const VkPhysicalDevice &device);
    SwapChainSupportDetails query_swap_chain_support(const VkPhysicalDevice &device);

    VkSurfaceFormatKHR choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR> &formats);
    VkPresentModeKHR choose_swap_present_mode(const std::vector<VkPresentModeKHR> &present_modes);
    VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR &capabilities);

    bool check_device_extension_support(const VkPhysicalDevice &device);
    // validation
    bool check_validation_layer_support();
    std::vector<const char*> get_required_extensions();

    void draw_frame();

public:
    Window() = default;
    ~Window() = default;

    void run();
};