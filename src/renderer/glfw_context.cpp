#include "glfw_context.h"

#include <stdexcept>

GlfwContext::GlfwContext() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    initialized = true;
}

GlfwContext::~GlfwContext() {
    initialized = false;

    glfwTerminate();
}

GLFWwindow* GlfwContext::create_window(uint32_t width, uint32_t height, const std::string& title) {
    if (!initialized) {
        throw std::runtime_error("Cannot create a window with an unitialized context.");
    }
    
    if (width <= 0) {
        throw std::runtime_error("Window width must be greater than zero.");
    }
    if (height <= 0) {
        throw std::runtime_error("Window height must be greater than zero.");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow* handle = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title.c_str(), nullptr, nullptr);

    if (!handle) {
        throw std::runtime_error("Failed to create window.");
    }

    return handle;
}

void GlfwContext::destroy_window(GLFWwindow* window) {
    if (window) {
        glfwDestroyWindow(window);
    }
}

bool GlfwContext::should_window_close(GLFWwindow* window) {
    return glfwWindowShouldClose(window);
}