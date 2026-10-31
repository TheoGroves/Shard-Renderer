#include "physical_device.h"

#include <iostream>
#include <stdexcept>
#include <vector>

namespace {

int rate_device(const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceMemoryProperties& memoryProperties) {
    int score = 0;

    // Favour dGPUs
    switch (properties.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            score += 10000;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            score += 5000;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            score += 2000;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            score += 1000;
            break;

        default:
            break;
    }

    // Favour higher Vulkan API versions
    score += static_cast<int>(VK_API_VERSION_MAJOR(properties.apiVersion)) * 100;
    score += static_cast<int>(VK_API_VERSION_MINOR(properties.apiVersion)) * 10;

    // Favour higher GPU memory
    VkDeviceSize device_local_memory = 0;

    for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; i++) {
        const auto& heap = memoryProperties.memoryHeaps[i];

        if (heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            device_local_memory += heap.size;
        }
    }

    const VkDeviceSize gb = 1024ULL * 1024 * 1024;
    score += static_cast<int>(device_local_memory / gb);

    return score;
}

}

PhysicalDevice::PhysicalDevice(VkInstance instance, VkSurfaceKHR surface) {
    pick_physical_device(instance, surface);
}

void PhysicalDevice::pick_physical_device(VkInstance instance, VkSurfaceKHR surface) {
    uint32_t device_count = 0;
    VkResult result = vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to enumerate physical devices.");
    }

    if (device_count == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support.");
    }

    std::vector<VkPhysicalDevice> devices(device_count);

    result = vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to retrieve physical devices.");
    }

    int best_score = -1;

    for (const auto& device : devices) {
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceMemoryProperties memory_properties{};

        vkGetPhysicalDeviceProperties(device, &properties);
        vkGetPhysicalDeviceMemoryProperties(device, &memory_properties);

        QueueFamilyIndices indices = find_queue_families(device, surface);

        if (!indices.is_complete()) {
            std::cout << "\nSkipping GPU:" << properties.deviceName << " (Missing required queue families)\n";
            continue;
        }

        int score = rate_device(properties, memory_properties);

        std::cout << "\nFound suitable Vulkan GPU:\n";
        std::cout << "Name: " << properties.deviceName << '\n';
        std:: cout << "Type: ";

        switch (properties.deviceType) {
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                std::cout << "Discrete GPU";
                break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                std::cout << "Integrated GPU";
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                std::cout << "Virtual GPU";
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                std::cout << "CPU";
                break;

            default:
                std::cout << "Other";
                break;
        }

        std::cout << '\n';

        std::cout << "Graphics queue family: " << indices.graphics_family.value() << '\n';
        std::cout << "Present queue family: " << indices.present_family.value() << '\n';

        std::cout << "Vulkan API: " << VK_API_VERSION_MAJOR(properties.apiVersion) << "." << VK_API_VERSION_MINOR(properties.apiVersion) << "." << VK_API_VERSION_PATCH(properties.apiVersion) << '\n';

        std::cout << "Score: " << score << '\n';

        if (score > best_score) {
            best_score = score;
            handle = device;
            queue_family_indices = indices;
        }
    }

    if (handle == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to select a physical device.");
    }

    VkPhysicalDeviceProperties selected_properties{};

    vkGetPhysicalDeviceProperties(handle, &selected_properties);

    std::cout << "\nSelected GPU: " << selected_properties.deviceName << '\n';
}

VkPhysicalDevice PhysicalDevice::get_handle() const {
    return handle;
}

const QueueFamilyIndices& PhysicalDevice::get_queue_family_indices() const {
    return queue_family_indices;
}

QueueFamilyIndices PhysicalDevice::find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;

    uint32_t queue_family_count = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

    uint32_t index = 0;

    for (const auto& queue_family : queue_families) {
        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphics_family = index;
        }

        VkBool32 present_support = false;

        VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface, &present_support);

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to query physical device presentation support.");
        }

        if (present_support) {
            indices.present_family = index;
        }

        if (indices.is_complete()) {
            break;
        } 

        index++;
    }

    return indices;
}