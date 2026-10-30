#include "window.h"

#include <stdexcept>

Window::Window(uint32_t width, uint32_t height, const std::string& title) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    handle = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), title.c_str(), nullptr, nullptr);

    if (!handle) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }
}

Window::~Window() {
    if (handle) {
        glfwDestroyWindow(handle);
    }

    glfwTerminate();
}

void Window::poll_events() const {
    glfwPollEvents();
}

bool Window::should_close() const {
    return glfwWindowShouldClose(handle);
}

GLFWwindow* Window::get_handle() const {
    return handle;
}