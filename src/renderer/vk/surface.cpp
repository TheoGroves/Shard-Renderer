#include "surface.h"

#include <stdexcept>

Surface::Surface(VkInstance instance, GLFWwindow* window) 
    : instance(instance) {
    
    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &handle);

    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan window surface.");
    }
}

Surface::~Surface() {
    if (handle != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(instance, handle, nullptr);
    }
}

VkSurfaceKHR Surface::get_handle() const {
    return handle;
}