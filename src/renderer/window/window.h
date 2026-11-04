#pragma once

#include "../glfw_context.h"

#include <GLFW/glfw3.h>
#include <string>
#include <memory>

class Window {
public:
    Window(std::shared_ptr<GlfwContext> ctx, uint32_t width, uint32_t height, const std::string& title);

    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void poll_events() const;

    bool should_close() const;

    GLFWwindow* get_handle() const;

private:
    std::shared_ptr<GlfwContext> context = nullptr;
    GLFWwindow* handle = nullptr;
};