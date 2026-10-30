#include "vulkan_instance.h"

#include <stdexcept>
#include <iostream>
#include <string>

VulkanInstance::VulkanInstance() {
    create_instance();
}

VulkanInstance::~VulkanInstance() {
    if (handle != VK_NULL_HANDLE) {
        vkDestroyInstance(handle, nullptr);
    }
}

void VulkanInstance::create_instance() {
    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "ShardRenderer";
    app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.pEngineName = "ShardRenderer";
    app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.apiVersion = VK_API_VERSION_1_0;

    uint32_t extension_count = 0;

    const char** extensions = glfwGetRequiredInstanceExtensions(&extension_count);

    if (!extensions) {
        throw std::runtime_error("Failed to get required Vulkan extensions");
    }

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = extension_count;
    create_info.ppEnabledExtensionNames = extensions;

    VkResult result = vkCreateInstance(&create_info, nullptr, &handle);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance: " + std::to_string(static_cast<int>(result)));
    }

    std::cout << "Vulkan initialized successfully.\n";
}

VkInstance VulkanInstance::get_handle() const {
    return handle;
}