#include "logical_device.h"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <format>
#include <string>

namespace {
const std::vector<const char*> device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
}

LogicalDevice::LogicalDevice(const PhysicalDevice& physical_device) {
    create_logical_device(physical_device);
    retrieve_queues(physical_device);
}

LogicalDevice::~LogicalDevice() {
    if (handle != VK_NULL_HANDLE) {
        vkDestroyDevice(handle, nullptr);
    }
}

void LogicalDevice::create_logical_device(const PhysicalDevice& physical_device) {
    const QueueFamilyIndices& indices = physical_device.get_queue_family_indices();

    if (!indices.is_complete()) {
        throw std::runtime_error("Physical device has incomplete queue family indices.");
    }

    std::vector<uint32_t> unique_queue_families = {
        indices.graphics_family.value(),
        indices.present_family.value()
    };

    std::sort(unique_queue_families.begin(), unique_queue_families.end());

    unique_queue_families.erase(std::unique(unique_queue_families.begin(), unique_queue_families.end()), unique_queue_families.end());

    constexpr float queue_priority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

    for (uint32_t queue_family : unique_queue_families) {
        VkDeviceQueueCreateInfo queue_create_info{};

        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;

        queue_create_infos.push_back(queue_create_info);
    }

    VkPhysicalDeviceFeatures device_features{};

    VkDeviceCreateInfo create_info{};

    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
    create_info.pQueueCreateInfos = queue_create_infos.data();
    create_info.pEnabledFeatures = &device_features;
    create_info.enabledExtensionCount = static_cast<uint32_t>(device_extensions.size());
    create_info.ppEnabledExtensionNames = device_extensions.data();

    VkResult result = vkCreateDevice(
        physical_device.get_handle(),
        &create_info,
        nullptr,
        &handle
    );

    if (result != VK_SUCCESS) {
        throw std::runtime_error(std::format("Failed to create logical device: {}", std::to_string(static_cast<int>(result))));
    }

    std::cout << "Vulkan logical device created successfully.\n";
}

void LogicalDevice::retrieve_queues(const PhysicalDevice& physical_device) {
    const QueueFamilyIndices& indices = physical_device.get_queue_family_indices();

    vkGetDeviceQueue(handle, indices.graphics_family.value(), 0, &graphics_queue);
    vkGetDeviceQueue(handle, indices.present_family.value(), 0, &present_queue);

    std::cout << "Graphics and presentation queues retrieved successfully.\n";
}

VkDevice LogicalDevice::get_handle() const {
    return handle;
}

VkQueue LogicalDevice::get_graphics_queue() const {
    return graphics_queue;
}

VkQueue LogicalDevice::get_present_queue() const {
    return present_queue;
}