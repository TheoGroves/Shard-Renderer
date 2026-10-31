#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

class Surface {
public:
    Surface(VkInstance instance, GLFWwindow* window);
    ~Surface();

    Surface(const Surface&) = delete;
    Surface& operator=(const Surface&) = delete;

    VkSurfaceKHR get_handle() const;

private:
    VkInstance instance = VK_NULL_HANDLE;
    VkSurfaceKHR handle = VK_NULL_HANDLE;
};