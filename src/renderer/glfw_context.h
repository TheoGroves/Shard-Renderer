#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

class GlfwContext {
public:
    GlfwContext();
    ~GlfwContext();

    GlfwContext(const GlfwContext&) = delete;
    GlfwContext& operator=(const GlfwContext&) = delete;

    GLFWwindow* create_window(uint32_t width, uint32_t height, const std::string& title);
    void destroy_window(GLFWwindow* window);
    bool should_window_close(GLFWwindow* window);
    void get_framebuffer_size(GLFWwindow* window, int* width, int* height);

private:
    bool initialized = false;
};