#pragma once

#include <vulkan/vulkan.h>

#include "queue_family_indices.h"

class PhysicalDevice {
public:
    explicit PhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

    PhysicalDevice(const PhysicalDevice&) = delete;
    PhysicalDevice& operator=(const PhysicalDevice&) = delete;

    VkPhysicalDevice get_handle() const;

    const QueueFamilyIndices& get_queue_family_indices() const;

private:
    VkPhysicalDevice handle = VK_NULL_HANDLE;

    QueueFamilyIndices queue_family_indices;

    void pick_physical_device(VkInstance instance, VkSurfaceKHR surface);

    QueueFamilyIndices find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface);
};