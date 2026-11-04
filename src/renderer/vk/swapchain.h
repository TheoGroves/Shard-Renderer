#pragma once

#include <vulkan/vulkan.h>

#include "physical_device.h"
#include "logical_device.h"
#include "surface.h"
#include "../window/window.h"

#include <vector>

class Swapchain {
public:
    Swapchain(const PhysicalDevice& physical_device, const LogicalDevice& logical_device, const Surface& surface, const Window& window);
    ~Swapchain();

    Swapchain(const Swapchain&) = delete;
    Swapchain& operator=(const Swapchain&) = delete;

    VkSwapchainKHR get_handle() const;

    const std::vector<VkImage>& get_images() const;

    VkFormat get_image_format() const;

    VkExtent2D get_extent() const;

private:
    struct SwapchainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    VkDevice device = VK_NULL_HANDLE;
    VkSwapchainKHR handle = VK_NULL_HANDLE;

    std::vector<VkImage> images;

    VkFormat image_format = VK_FORMAT_UNDEFINED;
    VkExtent2D extent{};

    void create_swapchain(const PhysicalDevice& physical_device, const LogicalDevice& logical_device, const Surface& surface, const Window& window);
    SwapchainSupportDetails query_swapchain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface) const;
    VkSurfaceFormatKHR choose_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats) const;
    VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR>& available_modes) const;
    VkExtent2D choose_extent(const VkSurfaceCapabilitiesKHR& capabilities, const Window& window) const;
};