#include "window.h"
#include "../glfw_context.h"

#include <stdexcept>

Window::Window(std::shared_ptr<GlfwContext> ctx, uint32_t width, uint32_t height, const std::string& title) {
    context = ctx;
    handle = context->create_window(width, height, title);
}

Window::~Window() {
    if (handle) {
        context->destroy_window(get_handle());
    }
}

void Window::poll_events() const {
    glfwPollEvents();
}

bool Window::should_close() const {
    return context->should_window_close(handle);
}

GLFWwindow* Window::get_handle() const {
    return handle;
}