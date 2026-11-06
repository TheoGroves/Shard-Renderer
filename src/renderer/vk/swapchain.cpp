#include "swapchain.h"

#include <algorithm>
#include <array>
#include <limits>
#include <stdexcept>
#include <string>
#include <iostream>

Swapchain::Swapchain(const PhysicalDevice& physical_device, const LogicalDevice& logical_device, const Surface& surface, const Window& window, VkSwapchainKHR old_swapchain) {
    create_swapchain(physical_device, logical_device, surface, window, old_swapchain);
}

Swapchain::~Swapchain() {
    if (handle != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(device, handle, nullptr);
    }
}

Swapchain::SwapchainSupportDetails Swapchain::query_swapchain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface) const {
    SwapchainSupportDetails details{};

    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details.capabilities);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to query surface capabilities.");
    }

    uint32_t format_count = 0;

    result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to query surface format count.");
    }

    details.formats.resize(format_count);

    if (format_count > 0) {
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, details.formats.data());

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to retrieve surface formats.");
        }
    }

    uint32_t present_mode_count = 0;

    result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to query presentation mode count.");
    }
    
    details.present_modes.resize(present_mode_count);

    if (present_mode_count > 0) {
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, details.present_modes.data());

        if (result != VK_SUCCESS) {
            throw std::runtime_error("Failed to retrieve presentation modes.");
        }
    }

    return details;
}

VkSurfaceFormatKHR Swapchain::choose_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats) const {
    for (const auto& format : available_formats) {
        if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }

    if (available_formats.empty()) {
        throw std::runtime_error("No supported surface formats available.");
    }

    return available_formats.front();
}

VkPresentModeKHR Swapchain::choose_present_mode(const std::vector<VkPresentModeKHR>& available_modes) const {
    for (const auto& mode : available_modes) {
        if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return mode;
        }
    }

    // Gauranteed to exist by Vulkan Spec
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Swapchain::choose_extent(const VkSurfaceCapabilitiesKHR& capabilities, const Window& window) const {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    }

    int width = 0;
    int height = 0;

    glfwGetFramebufferSize(window.get_handle(), &width, &height);

    VkExtent2D actual_extent{};

    actual_extent.width = std::clamp(
        static_cast<uint32_t>(std::max(width, 0)),
        capabilities.minImageExtent.width,
        capabilities.maxImageExtent.width
    );

    actual_extent.height = std::clamp(
        static_cast<uint32_t>(std::max(height, 0)),
        capabilities.minImageExtent.height,
        capabilities.maxImageExtent.height
    );

    return actual_extent;
}

void Swapchain::create_swapchain(const PhysicalDevice& physical_device, const LogicalDevice& logical_device, const Surface& surface, const Window& window, VkSwapchainKHR old_swapchain) {
    device = logical_device.get_handle();

    SwapchainSupportDetails support = query_swapchain_support(physical_device.get_handle(), surface.get_handle());

    if (support.formats.empty()) {
        throw std::runtime_error("No surface formats are supported.");
    }

    if (support.present_modes.empty()) {
        throw std::runtime_error("No presentation modes are supported.");
    }

    VkSurfaceFormatKHR surface_format = choose_surface_format(support.formats);
    VkPresentModeKHR present_mode = choose_present_mode(support.present_modes);
    extent = choose_extent(support.capabilities, window);

    if (support.capabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) {
        std::cout << "Surface supports colour attachment.\n";
    } else {
        throw std::runtime_error("Surface does not support colour attachment usage.");
    }

    uint32_t image_count = support.capabilities.minImageCount + 1;

    if (support.capabilities.maxImageCount > 0 && image_count > support.capabilities.maxImageCount) {
        image_count = support.capabilities.maxImageCount;
    }

    const QueueFamilyIndices& indices = physical_device.get_queue_family_indices();

    uint32_t queue_family_indices[] = {
        indices.graphics_family.value(),
        indices.present_family.value()
    };

    VkSwapchainCreateInfoKHR create_info{};

    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface.get_handle();

    create_info.minImageCount = image_count;
    create_info.imageFormat = surface_format.format;
    create_info.imageColorSpace = surface_format.colorSpace;
    create_info.imageExtent = extent;
    create_info.imageArrayLayers = 1;

    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    if (indices.graphics_family != indices.present_family) {
        create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    }
    else {
        create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;
        create_info.pQueueFamilyIndices = nullptr;
    }

    create_info.preTransform = support.capabilities.currentTransform;

    constexpr std::array<VkCompositeAlphaFlagBitsKHR, 4> composite_alpha_options = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
    };

    bool composite_alpha_found = false;

    for (const auto option : composite_alpha_options) {
        if (support.capabilities.supportedCompositeAlpha & option) {
            create_info.compositeAlpha = option;
            composite_alpha_found = true;
            break;
        }
    }

    if (!composite_alpha_found) {
        throw std::runtime_error("No supported composite alpha mode.");
    }

    create_info.presentMode = present_mode;
    create_info.clipped = VK_TRUE;
    create_info.oldSwapchain = old_swapchain;

    VkResult result = vkCreateSwapchainKHR(device, &create_info, nullptr, &handle);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create swapchain: " + std::to_string(static_cast<int>(result)));
    }

    uint32_t actual_image_count = 0;

    result = vkGetSwapchainImagesKHR(device, handle, &actual_image_count, nullptr);

    if (result == VK_SUCCESS && actual_image_count > 0) {
        images.resize(actual_image_count);

        result = vkGetSwapchainImagesKHR(device, handle, &actual_image_count, images.data());
    }

    if (result != VK_SUCCESS || actual_image_count == 0) {
        vkDestroySwapchainKHR(device, handle, nullptr);
        handle = VK_NULL_HANDLE;

        throw std::runtime_error("Failed to retrieve swapchain images.");
    }

    images.resize(actual_image_count);

    image_format = surface_format.format;
}

VkSwapchainKHR Swapchain::get_handle() const {
    return handle;
}

const std::vector<VkImage>& Swapchain::get_images() const {
    return images;
}

VkFormat Swapchain::get_image_format() const {
    return image_format;
}

VkExtent2D Swapchain::get_extent() const {
    return extent;
}