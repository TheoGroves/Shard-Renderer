#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VulkanInstance {
public:
    VulkanInstance();
    ~VulkanInstance();

    VulkanInstance(const VulkanInstance&) = delete;
    VulkanInstance& operator=(const VulkanInstance*) = delete;

    VkInstance get_handle() const;

private:
    VkInstance handle = VK_NULL_HANDLE;

    void create_instance();
};