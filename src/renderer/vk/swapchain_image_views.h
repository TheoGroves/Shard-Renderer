#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "logical_device.h"
#include "swapchain.h"

class SwapchainImageViews {
public:
    SwapchainImageViews(const LogicalDevice& logical_device, const Swapchain& swapchain);
    ~SwapchainImageViews();

    SwapchainImageViews(const SwapchainImageViews&) = delete;
    SwapchainImageViews& operator=(const SwapchainImageViews&) = delete;

    const std::vector<VkImageView>& get_image_views() const;

private:
    VkDevice device = VK_NULL_HANDLE;

    std::vector<VkImageView> image_views;

    void create_image_views(const Swapchain& swapchain);
};