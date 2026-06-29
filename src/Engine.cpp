#include "Engine.h"
#include <format>
#include <iostream>

constexpr std::string engineVersion = "0.0.3";

#define ANSI_RESET   "\033[0m"
#define ANSI_YELLOW  "\033[33m"
#define ANSI_RED     "\033[31m"

void Engine::GLFWErrorCallback(int error, const char* description)
{
    LogError(std::format("GLFW {}: {}", error, description));
}

void APIENTRY Engine::GLDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
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

void Engine::LogMessage(std::string_view message)
{
    std::cout << ANSI_RESET << "[SYSTEM] " << message << "\n";
}

void Engine::LogWarning(std::string_view warning)
{
    std::cout << ANSI_YELLOW << "[WARNING] " << warning << ANSI_RESET << "\n";
}

void Engine::LogError(std::string_view error)
{
    std::cerr << ANSI_RED << "[ERROR] " << error << ANSI_RESET << "\n";
}

bool Engine::Initialize(unsigned int screenWidth, unsigned int screenHeight, std::string title)
{
    LogMessage(std::format("SHARD RENDERER v{}", engineVersion));
    LogMessage(std::format("Compiled on: {} at {}", (std::string)__DATE__,  __TIME__));

    glfwSetErrorCallback(GLFWErrorCallback);

    if (!glfwInit())
    {
        LogError("Failed to initialize GLFW.");
        return false;
    }
    LogMessage("GLFW initialized successfully.");

    if (!mWindow.Create(screenWidth, screenHeight, title))
    {
        glfwTerminate();
        LogError("Failed to create OpenGL context.");
        return false;
    }

    LogMessage(std::format("Created Window of size {}x{}", screenWidth, screenHeight));

    if (glewInit() != GLEW_OK)
    {
        LogError("Failed to initialize GLEW.");
        glfwTerminate();
        return false;
    }
    LogMessage("GLEW initialized successfully.");

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLDebug, nullptr);

    const GLubyte* vendor   = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version  = glGetString(GL_VERSION);

    LogMessage(std::format("GPU Vendor: {}", reinterpret_cast<const char*>(vendor)));
    LogMessage(std::format("Renderer  : {}", reinterpret_cast<const char*>(renderer)));
    LogMessage(std::format("GL Version: {}", reinterpret_cast<const char*>(version)));

    glClearColor(0.05f, 0.07f, 0.09f, 1.0f);

    mShader.LoadFromFile("assets/shaders/basic.vert", "assets/shaders/basic.frag");

    return true;
}

void Engine::BeginFrame()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Engine::EndFrame()
{
    mWindow.SwapBuffers();
    mWindow.PollEvents();
}

int Engine::CreateMesh()
{
    float vertices[] = {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2
    };

    Mesh mesh;

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);
    
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &mesh.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    mesh.indexCount = 3;

    int id = mNextMeshID++;
    mMeshes[id] = mesh;

    return id;
}

void Engine::DrawMesh(int meshID)
{
    auto it = mMeshes.find(meshID);
    if (it == mMeshes.end())
        return;
    
    Mesh& mesh = it->second;

    mShader.Use();

    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
}

bool Engine::ShouldClose() const
{
    return mWindow.ShouldClose();
}

void Engine::Shutdown()
{
    LogMessage("Engine shutting down.");

    mWindow.Destroy();
    glfwTerminate();
    mRunning = false;

    LogMessage("Engine shutdown complete.");
}