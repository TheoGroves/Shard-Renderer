#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class VulkanInstance {
public:
    VulkanInstance();
    ~VulkanInstance();

    VulkanInstance(const VulkanInstance&) = delete;
    VulkanInstance& operator=(const VulkanInstance&) = delete;

    VkInstance get_handle() const;

private:
    VkInstance handle = VK_NULL_HANDLE;

    VkDebugUtilsMessengerEXT debug_messenger = VK_NULL_HANDLE;

    void create_instance();
    void setup_debug_messenger();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
        VkDebugUtilsMessageTypeFlagsEXT message_type,
        const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
        void* user_data
    );

    VkDebugUtilsMessengerCreateInfoEXT get_debug_messenger_info() const;
};