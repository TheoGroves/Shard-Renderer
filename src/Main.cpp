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

constexpr std::string engineVersion = "0.0.2";

constexpr unsigned int screenWidth = 1920;
constexpr unsigned int screenHeight = 1080;

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

void GLFWErrorCallback(int error, const char* description)
{
    LogError(std::format("GLFW {}: {}", error, description));
}

void APIENTRY GLDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        LogError(message);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        LogWarning(message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        LogWarning(message);
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        LogMessage(message);
        break;
    }
}

int main(void)
{
    LogMessage(std::format("SHARD RENDERER v{}", engineVersion));
    LogMessage(std::format("Compiled on: {} at {}", (std::string)__DATE__,  __TIME__));

    GLFWwindow* window;

    if (!glfwInit())
    {
        LogError("Failed to initialize GLFW.");
        return -1;
    }
    LogMessage("GLFW initialized successfully.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(screenWidth, screenHeight, "Shard Renderer", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        LogError("Failed to create OpenGL context.");
        return -1;
    }

    LogMessage(std::format("Created Window of size {}x{}", screenWidth, screenHeight);

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        LogError("Failed to initialize GLEW.");
        glfwTerminate();
        return -1;
    }
    LogMessage("GLEW initialized successfully.");

    glfwSetErrorCallback(GLFWErrorCallback);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLDebug, nullptr);

    const GLubyte* vendor   = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version  = glGetString(GL_VERSION);

    LogMessage(std::format("GPU Vendor: {}", reinterpret_cast<const char*>(vendor)));
    LogMessage(std::format("Renderer  : {}", reinterpret_cast<const char*>(renderer)));
    LogMessage(std::format("GL Version: {}", reinterpret_cast<const char*>(version)));

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