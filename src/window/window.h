#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

class Window {
public:
    Window(uint32_t width, uint32_t height, const std::string& title);


    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void poll_events() const;

    bool should_close() const;

    GLFWwindow* get_handle() const;

private:
    GLFWwindow* handle = nullptr;
};