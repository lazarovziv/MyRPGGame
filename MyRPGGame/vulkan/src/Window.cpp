#include "Window.hpp"

void Window::run() {
    init_window();
    init_vulkan();
    main_loop();
    cleanup();
}

void Window::init_window() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window_ = glfwCreateWindow(WIDTH, HEIGHT, "vulkan", nullptr, nullptr);
}

void Window::init_vulkan() {
    create_instance();
    create_surface();
    pick_physical_device();
    create_logical_device();
    create_swap_chain();

    create_image_views();
    create_render_pass();
    create_graphics_pipeline();
    create_frame_buffers();

    create_command_pool();
    create_command_buffers();

    create_sync_objects();
}

void Window::main_loop() {
    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
        draw_frame();
    }

    vkDeviceWaitIdle(device);
}

void Window::cleanup() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        vkDestroySemaphore(device, image_available_semaphores[i], nullptr);
        vkDestroySemaphore(device, render_finished_semaphores[i], nullptr);
        vkDestroyFence(device, in_flight_fences[i], nullptr);
    }

    vkDestroyCommandPool(device, command_pool, nullptr);

    for (auto frame_buffer : swap_chain_frame_buffers) {
        vkDestroyFramebuffer(device, frame_buffer, nullptr);
    }

    vkDestroyPipeline(device, graphics_pipeline, nullptr);
    vkDestroyPipelineLayout(device, pipeline_layout, nullptr);
    vkDestroyRenderPass(device, render_pass, nullptr);

    for (auto image_view : swap_chain_image_views) {
        vkDestroyImageView(device, image_view, nullptr);
    }

    vkDestroySwapchainKHR(device, swap_chain, nullptr);
    vkDestroyDevice(device, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window_);

    glfwTerminate();
}

void Window::create_instance() {
    if (enable_validation_layers && !check_validation_layer_support()) {
        throw std::runtime_error("Validation layers requested, but not available!");
    }

	VkApplicationInfo app_info{};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "Vulkan";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "No Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    // include validation layers if enabled
    if (enable_validation_layers) {
        create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        create_info.ppEnabledLayerNames = validation_layers.data();
    } else create_info.enabledLayerCount = 0;

    std::vector<const char*> extensions = get_required_extensions();

    extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    create_info.ppEnabledExtensionNames = extensions.data();

    VkResult result = vkCreateInstance(&create_info, nullptr, &instance);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create an instance!");
    }
    
}

void Window::create_surface() {
    VkResult result = glfwCreateWindowSurface(instance, window_, nullptr, &surface);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface!");
    }
}

void Window::create_swap_chain() {
    SwapChainSupportDetails swap_chain_support = query_swap_chain_support(physical_device);

    VkSurfaceFormatKHR surface_format = choose_swap_surface_format(swap_chain_support.formats);
    VkPresentModeKHR surface_present_mode = choose_swap_present_mode(swap_chain_support.present_modes);
    VkExtent2D extent = choose_swap_extent(swap_chain_support.capabilities);

    // min number of images to store in the swap chain
    uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;
    // limiting max number of images in the swap chain
    if (swap_chain_support.capabilities.maxImageCount > 0 &&
        image_count > swap_chain_support.capabilities.maxImageCount) {
            image_count = swap_chain_support.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    // for 3d application it should be different from 1
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = find_queue_families(physical_device);
    uint32_t queue_family_indices[] = { indices.graphics_family.value(), indices.present_family.value() };
    // setting queue transfer policy between queues
    if (indices.graphics_family != indices.present_family) {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }
    
    // no transformations to images in the swap chain
    create_info.preTransform = swap_chain_support.capabilities.currentTransform;
    // ignoring the alpha channel
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = surface_present_mode;
    create_info.clipped = VK_TRUE;
    // relevant to when window is resized and swap chain needs to be recreated
    create_info.oldSwapchain = VK_NULL_HANDLE;

    VkResult result = vkCreateSwapchainKHR(device, &create_info, nullptr, &swap_chain);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create the swap chain!");
    }

    // swap chain created
    vkGetSwapchainImagesKHR(device, swap_chain, &image_count, nullptr);
    swap_chain_images.resize(image_count);
    vkGetSwapchainImagesKHR(device, swap_chain, &image_count, swap_chain_images.data());

    swap_chain_image_format = surface_format.format;
    swap_chain_extent = extent;
}

void Window::create_frame_buffers() {
    swap_chain_frame_buffers.resize(swap_chain_image_views.size());
    for (size_t i = 0; i < swap_chain_image_views.size(); ++i) {
        VkImageView attachments[] = { swap_chain_image_views[i] };

        VkFramebufferCreateInfo frame_buffer_create_info{};
        frame_buffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frame_buffer_create_info.renderPass = render_pass;
        frame_buffer_create_info.attachmentCount = 1;
        frame_buffer_create_info.pAttachments = attachments;
        frame_buffer_create_info.width = swap_chain_extent.width;
        frame_buffer_create_info.height = swap_chain_extent.height;
        frame_buffer_create_info.layers = 1;

        VkResult result = vkCreateFramebuffer(device, &frame_buffer_create_info, nullptr, &swap_chain_frame_buffers[i]);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create frame buffer!");
        }
    }
}

void Window::create_image_views() {
    swap_chain_image_views.resize(swap_chain_images.size());
    for (size_t i = 0; i < swap_chain_images.size(); ++i) {
        VkImageViewCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_info.image = swap_chain_images[i];

        create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        create_info.format = swap_chain_image_format;
        // default mapping of image's channels
        create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        create_info.subresourceRange.baseMipLevel = 0;
        create_info.subresourceRange.levelCount = 1;
        create_info.subresourceRange.baseArrayLayer = 0;
        create_info.subresourceRange.layerCount = 1;

        VkResult result = vkCreateImageView(device, &create_info, nullptr, &swap_chain_image_views[i]);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to create image views!");
        }
    }
}

void Window::create_graphics_pipeline() {
    auto vert_shader_code = shaders::read_file("../../shaders/vert.spv");
    auto frag_shader_code = shaders::read_file("../../shaders/frag.spv");

    VkShaderModule vert_shader_module = shaders::create_shader_module(device, vert_shader_code);
    VkShaderModule frag_shader_module = shaders::create_shader_module(device, frag_shader_code);

    VkPipelineShaderStageCreateInfo vert_shader_create_info{};
    vert_shader_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    // in which pipeline stage will the shader be used
    vert_shader_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vert_shader_create_info.module = vert_shader_module;
    vert_shader_create_info.pName = "main";

    VkPipelineShaderStageCreateInfo frag_shader_create_info{};
    frag_shader_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_shader_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_shader_create_info.module = frag_shader_module;
    frag_shader_create_info.pName = "main";

    VkPipelineShaderStageCreateInfo shader_stages[] = { vert_shader_create_info, frag_shader_create_info };

    VkPipelineVertexInputStateCreateInfo vertex_create_info{};
    vertex_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_create_info.vertexBindingDescriptionCount = 0;
    vertex_create_info.pVertexBindingDescriptions = nullptr;
    vertex_create_info.vertexAttributeDescriptionCount = 0;
    vertex_create_info.pVertexAttributeDescriptions = nullptr;

    VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info{};
    input_assembly_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly_create_info.primitiveRestartEnable = VK_FALSE;

    VkViewport view_port{};
    view_port.x = 0.0f;
    view_port.y = 0.0f;
    // the swap chain images will be used as framebuffers
    view_port.width = (float) swap_chain_extent.width;
    view_port.height = (float) swap_chain_extent.height;
    view_port.minDepth = 0.0f;
    view_port.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = swap_chain_extent;
    
    std::vector<VkDynamicState> dynamic_states = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynamic_state_create_info{};
    dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state_create_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
    dynamic_state_create_info.pDynamicStates = dynamic_states.data();

    VkPipelineViewportStateCreateInfo view_port_create_info{};
    view_port_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    view_port_create_info.viewportCount = 1;
    view_port_create_info.scissorCount = 1;

    // VkPipelineViewportStateCreateInfo view_port_state_create_info{};

    VkPipelineRasterizationStateCreateInfo rasterizer_create_info{};
    rasterizer_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer_create_info.depthClampEnable = VK_FALSE;
    rasterizer_create_info.rasterizerDiscardEnable = VK_FALSE;
    rasterizer_create_info.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer_create_info.lineWidth = 1.0f;
    rasterizer_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer_create_info.depthBiasEnable = VK_FALSE;
    rasterizer_create_info.depthBiasConstantFactor = 0.0f;
    rasterizer_create_info.depthBiasClamp = 0.0f;
    rasterizer_create_info.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampling_create_info{};
    multisampling_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling_create_info.sampleShadingEnable = VK_FALSE;
    multisampling_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling_create_info.minSampleShading = 1.0f;
    multisampling_create_info.pSampleMask = nullptr;
    multisampling_create_info.alphaToCoverageEnable = VK_FALSE;
    multisampling_create_info.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState color_blend_attachment{};
    color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | 
        VK_COLOR_COMPONENT_G_BIT | 
        VK_COLOR_COMPONENT_B_BIT | 
        VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo color_blending_create_info{};
    color_blending_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blending_create_info.logicOpEnable = VK_FALSE;
    color_blending_create_info.logicOp = VK_LOGIC_OP_COPY;
    color_blending_create_info.attachmentCount = 1;
    color_blending_create_info.pAttachments = &color_blend_attachment;

    VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
    pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    
    VkResult result = vkCreatePipelineLayout(device, &pipeline_layout_create_info, nullptr, &pipeline_layout);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipeline_create_info{};
    pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_create_info.stageCount = 2;
    pipeline_create_info.pStages = shader_stages;
    pipeline_create_info.pVertexInputState = &vertex_create_info;
    pipeline_create_info.pInputAssemblyState = &input_assembly_create_info;
    pipeline_create_info.pViewportState = &view_port_create_info;
    pipeline_create_info.pRasterizationState = &rasterizer_create_info;
    pipeline_create_info.pMultisampleState = &multisampling_create_info;
    pipeline_create_info.pDepthStencilState = nullptr;
    pipeline_create_info.pColorBlendState = &color_blending_create_info;
    pipeline_create_info.pDynamicState = &dynamic_state_create_info;

    pipeline_create_info.layout = pipeline_layout;
    pipeline_create_info.renderPass = render_pass;
    pipeline_create_info.subpass = 0;

    // can be used when another pipeline has same values as this pipeline
    pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
    pipeline_create_info.basePipelineIndex = -1;

    VkResult graphics_result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1,
     &pipeline_create_info, nullptr, &graphics_pipeline);
    if (graphics_result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create graphics pipeline!");
    }

    // compilation and linking of the spir-v bytecode doesnt happen until the graphics pipeline is created
    vkDestroyShaderModule(device, frag_shader_module, nullptr);
    vkDestroyShaderModule(device, vert_shader_module, nullptr);
}

void Window::create_render_pass() {
    VkAttachmentDescription color_attachment{};
    color_attachment.format = swap_chain_image_format;
    // relevant to multisampling
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    // not doing anything with the stencil buffer so no need to to load or store
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // images should be ready for presentation using the swap chain after rendering
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference color_attachment_ref{};
    // index of the attachment descriptions array and its' size is 1
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    // referenced from the fragment shader
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    // need to wait for the swap chain to finish reading from the image before accessing it so waiting on the color attachment
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    // the operations that should wait are in the color attachment stage and are writing the color attachment
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo render_pass_create_info{};
    render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_create_info.attachmentCount = 1;
    render_pass_create_info.pAttachments = &color_attachment;
    render_pass_create_info.subpassCount = 1;
    render_pass_create_info.pSubpasses = &subpass;

    render_pass_create_info.dependencyCount = 1;
    render_pass_create_info.pDependencies = &dependency;

    VkResult result = vkCreateRenderPass(device, &render_pass_create_info, nullptr, &render_pass);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create render pass!");
    }
}

void Window::create_command_pool() {
    QueueFamilyIndices queue_family_indices = find_queue_families(physical_device);

    VkCommandPoolCreateInfo pool_create_info{};
    pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_create_info.queueFamilyIndex = queue_family_indices.graphics_family.value();

    VkResult result = vkCreateCommandPool(device, &pool_create_info, nullptr, &command_pool);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool!");
    }
}

void Window::create_command_buffers() {
    command_buffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocate_info{};
    allocate_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocate_info.commandPool = command_pool;
    allocate_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocate_info.commandBufferCount = (uint32_t) command_buffers.size();

    VkResult result = vkAllocateCommandBuffers(device, &allocate_info, command_buffers.data());
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers!");
    }
}

void Window::create_sync_objects() {
    image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
    in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphore_info{};
    semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_info{};
    fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    // setting the fence as signaled for the first frame (!!!)
    fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        if (vkCreateSemaphore(device, &semaphore_info, nullptr, &image_available_semaphores[i]) ||
            vkCreateSemaphore(device, &semaphore_info, nullptr, &render_finished_semaphores[i]) ||
            vkCreateFence(device, &fence_info, nullptr, &in_flight_fences[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create synchoronization objects for a frame!");
        }
    }
}

void Window::record_command_buffer(const VkCommandBuffer &command_buffer, const uint32_t image_index) {
    VkCommandBufferBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = 0;
    begin_info.pInheritanceInfo = nullptr;

    VkResult result = vkBeginCommandBuffer(command_buffer, &begin_info);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffers!");
    }

    VkRenderPassBeginInfo render_pass_begin_info{};
    render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_begin_info.renderPass = render_pass;
    render_pass_begin_info.framebuffer = swap_chain_frame_buffers[image_index];
    render_pass_begin_info.renderArea.offset = { 0, 0 };
    render_pass_begin_info.renderArea.extent = swap_chain_extent;

    VkClearValue clear_color = {{{ 0.0f, 0.0f, 0.0f, 1.0f }}};
    render_pass_begin_info.clearValueCount = 1;
    render_pass_begin_info.pClearValues = &clear_color;

    vkCmdBeginRenderPass(command_buffer, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
    // graphics pipeline and not compute
    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline);

    VkViewport view_port{};
    view_port.x = 0.0f;
    view_port.y = 0.0f;
    // the swap chain images will be used as framebuffers
    view_port.width = (float) swap_chain_extent.width;
    view_port.height = (float) swap_chain_extent.height;
    view_port.minDepth = 0.0f;
    view_port.maxDepth = 1.0f;
    vkCmdSetViewport(command_buffer, 0, 1, &view_port);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = swap_chain_extent;
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);

    vkCmdDraw(command_buffer, 3, 1, 0, 0);

    vkCmdEndRenderPass(command_buffer);

    VkResult command_buffer_end_result = vkEndCommandBuffer(command_buffer);
    if (command_buffer_end_result != VK_SUCCESS) {
        throw std::runtime_error("Failed to record comman buffer!");
    }
}

void Window::pick_physical_device() {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
    // if no devices found
    if (device_count == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    // creating a vector to store all gpus
    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    // ordered map (by key) to be able to get highest scoring device
    std::multimap<int, VkPhysicalDevice> candidates;

    for (const auto& device : devices) {
        if (!is_device_suitable(device)) continue;

        int score = rate_device_suitability(device);
        candidates.insert({ score, device });
        // candidates.insert(std::make_pair(score, device));   
    }

    // getting the highest scoring gpu
    if (candidates.rbegin()->first > 0) {
        physical_device = candidates.rbegin()->second;
    } else throw std::runtime_error("Failed to find a suitable GPU!");

}

bool Window::is_device_suitable(const VkPhysicalDevice &device) {
    QueueFamilyIndices indices = find_queue_families(device);

    bool extensions_supported = check_device_extension_support(device);
    // checking swap chain support AFTER checking extension is available
    bool swap_chain_adequate = false;
    if (extensions_supported) {
        SwapChainSupportDetails swap_chain_support = query_swap_chain_support(device);
        // adequate only if both vectors are NOT empty
        swap_chain_adequate = !(swap_chain_support.formats.empty() || swap_chain_support.present_modes.empty());
    }

    return indices.is_complete() && extensions_supported && swap_chain_adequate;
}

int Window::rate_device_suitability(const VkPhysicalDevice &device) {
    // enumerating the device
    VkPhysicalDeviceProperties device_properties;
    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceProperties(device, &device_properties);
    vkGetPhysicalDeviceFeatures(device, &device_features);
    
    int score = 0;
    // dedicated gpu
    if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }
    // max size of textures
    score += device_properties.limits.maxImageDimension2D;

    // must have a geometry shader
    if (!device_features.geometryShader) return 0;

    return score;
}

void Window::create_logical_device() {
    QueueFamilyIndices indices = find_queue_families(physical_device);

    // since there's a possibility for a device to not support presentation, we'll create a device for each queue family
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    std::set<uint32_t> unique_queue_families = { 
        indices.graphics_family.value(),
        indices.present_family.value() 
    };

    // equal priority for all queue families
    float queue_priority = 1.0f;

    for (uint32_t queue_family : unique_queue_families) {
        VkDeviceQueueCreateInfo queue_create_info{};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(queue_create_info);
    }

    // getting optional features for the gpu we're using
    VkPhysicalDeviceFeatures device_features{};

    VkDeviceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    // add pointers to the queue creation info and for the features info
    create_info.pQueueCreateInfos = queue_create_infos.data();
    create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
    create_info.pEnabledFeatures = &device_features;
    // TODO: specify extensions and validation layers for this device
    create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
    create_info.ppEnabledExtensionNames = device_extensions.data();

    if (enable_validation_layers) {
        create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
        create_info.ppEnabledLayerNames = validation_layers.data();
    } else {
        create_info.enabledLayerCount = 0;
    }

    // create the logical device
    VkResult result = vkCreateDevice(physical_device, &create_info, nullptr, &device);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create a logical device!");
    }
    // logical device created

    // retrieving queue handles for each queue family, but we're using a single queue so setting index to 0
    vkGetDeviceQueue(device, indices.graphics_family.value(), 0, &graphics_queue);
    vkGetDeviceQueue(device, indices.present_family.value(), 0, &present_queue);
}

QueueFamilyIndices Window::find_queue_families(const VkPhysicalDevice &device) {
    QueueFamilyIndices indices;

    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
    // storing all data about queue families for given device, including the type of operations that are supported
    // and the number of queues that can be created based on the family
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    int i = 0;
    for (const auto& queue_family : queue_families) {
        VkBool32 present_support = false;
        // presentation support implies swap chain extension support
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
        if (present_support) indices.present_family = i;

        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) indices.graphics_family = i;

        // if found suitable device with relevant queue families and number of queues
        if (indices.is_complete()) break;

        ++i;
    }

    return indices;
}

SwapChainSupportDetails Window::query_swap_chain_support(const VkPhysicalDevice &device) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t format_count = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);
    if (format_count != 0) {
        // changing the initial size of the formats vector to match the enumeration findings
        details.formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device,
            surface,
            &format_count,
            details.formats.data()
        );
    }
    
    uint32_t present_mode_count = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);
    if (present_mode_count != 0) {
        // doing the same for the present modes vector
        details.present_modes.resize(present_mode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device,
            surface,
            &present_mode_count,
            details.present_modes.data()
        );
    }

    return details;
}

VkSurfaceFormatKHR Window::choose_swap_surface_format(const std::vector<VkSurfaceFormatKHR> &formats) {
    for (const auto& format : formats) {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && 
            format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return format;
            }
    }
    return formats[0];
}

VkPresentModeKHR Window::choose_swap_present_mode(const std::vector<VkPresentModeKHR> &present_modes) {
    // giving priority for mailbox present mode
    for (const auto& present_mode : present_modes) {
        if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return present_mode;
        }
    }
    // fifo is available on all systems and more energy efficient (good for mobile games for instance)
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Window::choose_swap_extent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }

    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);

    VkExtent2D actual_extent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };

    // clamping the width and height received from the current extent to match the screen resolution
    actual_extent.width = std::clamp(actual_extent.width,
        capabilities.minImageExtent.width,
        capabilities.maxImageExtent.width
    );
    actual_extent.height = std::clamp(actual_extent.height,
        capabilities.minImageExtent.height,
        capabilities.maxImageExtent.height
    );

    return actual_extent;
}

bool Window::check_device_extension_support(const VkPhysicalDevice &device) {
    uint32_t extension_count = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

    // filling required extensions with device extensions defined in Window.hpp
    std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

    // removing all extensions from required extensions
    for (const auto& extension : available_extensions) {
        required_extensions.erase(extension.extensionName);
    }
    // if the list is not empty, there's a required extensions that's not available
    return required_extensions.empty();
}

bool Window::check_validation_layer_support() {
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    for (const char *layer_name : validation_layers) {
        bool layer_found = false;

        for (const auto& layer_properties : available_layers) {
            if (strcmp(layer_name, layer_properties.layerName) == 0) {
                layer_found = true;
                break;
            }
        }
        if (!layer_found) return false;
    }

    return true;
}

std::vector<const char*> Window::get_required_extensions() {
    uint32_t glfw_extension_count = 0;
    const char **glfw_extensions;

    // includes surface and other extensions
    glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

    std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

    if (enable_validation_layers) extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    return extensions;
}

void Window::draw_frame() {
    // wait until previous frame finishes
    vkWaitForFences(device, 1, &in_flight_fences[current_frame], VK_TRUE, UINT64_MAX);
    // reset the fence to unsignaled state
    vkResetFences(device, 1, &in_flight_fences[current_frame]);

    uint32_t image_index = -1;
    vkAcquireNextImageKHR(device, swap_chain, UINT64_MAX, image_available_semaphores[current_frame], VK_NULL_HANDLE, &image_index);

    vkResetCommandBuffer(command_buffers[current_frame], 0);
    record_command_buffer(command_buffers[current_frame], image_index);

    VkSubmitInfo submit_info{};
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore wait_semaphores[] = { image_available_semaphores[current_frame] };
    VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = wait_semaphores;
    submit_info.pWaitDstStageMask = wait_stages;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &command_buffers[current_frame];

    VkSemaphore signal_semaphores[] = { render_finished_semaphores[current_frame] };
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = signal_semaphores;

    if (vkQueueSubmit(graphics_queue, 1, &submit_info, in_flight_fences[current_frame]) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer!");
    }

    VkPresentInfoKHR present_info{};
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    // how many and which semaphores to wait on before presentation
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = signal_semaphores;

    VkSwapchainKHR swap_chains[] = { swap_chain };
    present_info.swapchainCount = 1;
    present_info.pSwapchains = swap_chains;
    present_info.pImageIndices = &image_index;
    // helpful when using multiple swapchains
    present_info.pResults = nullptr;

    vkQueuePresentKHR(present_queue, &present_info);

    current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}
