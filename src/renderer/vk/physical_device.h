#pragma once

#include <vulkan/vulkan.h>

class PhysicalDevice {
public:
    explicit PhysicalDevice(VkInstance instance);

    PhysicalDevice(const PhysicalDevice&) = delete;
    PhysicalDevice& operator=(const PhysicalDevice&) = delete;

    VkPhysicalDevice get_handle() const;

private:
    VkPhysicalDevice handle = VK_NULL_HANDLE;

    void pick_physical_device(VkInstance instance);
};