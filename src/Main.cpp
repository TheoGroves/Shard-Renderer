#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")

#define ANSI_RESET   "\033[0m"
#define ANSI_YELLOW  "\033[33m"
#define ANSI_RED     "\033[31m"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <string_view>
#include <format>

extern "C" {
    // Find best NVIDIA GPU
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
    
    // Find best AMD GPU
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

constexpr std::string engineVersion = "0.0.1";

constexpr unsigned int screenWidth = 640;
constexpr unsigned int screenHeight = 480;

void LogMessage(std::string_view message)
{
    std::cout << ANSI_RESET << "[SYSTEM] " << message << "\n";
}

void LogWarning(std::string_view warning)
{
    std::cout << ANSI_YELLOW << "[WARNING] " << warning << ANSI_RESET << "\n";
}

void LogError(std::string_view error)
{
    std::cerr << ANSI_RED << "[ERROR] " << error << ANSI_RESET << "\n";
}

int main(void)
{
    LogMessage("SHARD RENDERER v" + engineVersion);
    LogMessage("Compiled on: " + (std::string)__DATE__ + " at " + __TIME__);

    GLFWwindow* window;

    if (!glfwInit())
    {
        LogError("Failed to initialize GLFW.");
        return -1;
    }
    LogMessage("GLFW initialized successfully.");

    window = glfwCreateWindow(screenWidth, screenHeight, "Shard Renderer", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        LogError("Failed to create OpenGL context.");
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        LogError("Failed to initialize GLEW.");
        glfwTerminate();
        return -1;
    }
    LogMessage("GLEW initialized successfully.");

    const GLubyte* vendor   = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version  = glGetString(GL_VERSION);

    LogMessage(std::format("GPU Vendor  : {}", reinterpret_cast<const char*>(vendor)));
    LogMessage(std::format("Renderer    : {}", reinterpret_cast<const char*>(renderer)));
    LogMessage(std::format("GL Version  : {}", reinterpret_cast<const char*>(version)));

    glClearColor(0.05f, 0.07f, 0.09f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}