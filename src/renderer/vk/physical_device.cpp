#include "physical_device.h"

#include <iostream>
#include <stdexcept>
#include <vector>

namespace {

int rate_device(const VkPhysicalDeviceProperties& properties, const VkPhysicalDeviceMemoryProperties2& memoryProperties) {
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

    for (uint32_t i = 0; i < memoryProperties.memoryProperties.memoryHeapCount; i++) {
        const auto& heap = memoryProperties.memoryProperties.memoryHeaps[i];

        if (heap.flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            device_local_memory += heap.size;
        }
    }

    const VkDeviceSize gb = 1024ULL * 1024 * 1024;
    score += static_cast<int>(device_local_memory / gb);

    return score;
}

}

PhysicalDevice::PhysicalDevice(VkInstance instance) {
    pick_physical_device(instance);
}

void PhysicalDevice::pick_physical_device(VkInstance instance) {
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    if (device_count == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support.");
    }

    std::vector<VkPhysicalDevice> devices(device_count);

    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    int best_score = -1;

    for (const auto& device : devices) {
        VkPhysicalDeviceProperties properties{};
        VkPhysicalDeviceMemoryProperties2 memory_properties{};

        vkGetPhysicalDeviceProperties(device, &properties);
        vkGetPhysicalDeviceMemoryProperties2(device, &memory_properties);

        int score = rate_device(properties, memory_properties);

        std::cout << "\nFound Vulkan GPU:\n";
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

        std::cout << "Vulkan API: " << VK_API_VERSION_MAJOR(properties.apiVersion) << "." << VK_API_VERSION_MINOR(properties.apiVersion) << "." << VK_API_VERSION_PATCH(properties.apiVersion) << '\n';

        std::cout << "Score: " << score << '\n';

        if (score > best_score) {
            best_score = score;
            handle = device;
        }
    }

    if (handle == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to select a physical device.");
    }

    VkPhysicalDeviceProperties selected_properties{};

    vkGetPhysicalDeviceProperties(handle, &selected_properties);

    std::cout << "\nSelected GPU:" << selected_properties.deviceName << '\n';
}

VkPhysicalDevice PhysicalDevice::get_handle() const {
    return handle;
}