#include "Engine.h"
#include <format>
#include <iostream>

constexpr std::string engineVersion = "0.0.6";

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

void Engine::UpdateMat4(const std::string& uniform, const Mat4& matrix)
{
    mShader.SetMat4(uniform, matrix);
}

Input Engine::GetInput()
{
    Input input{};

    GLFWwindow* window = mWindow.GetNativeWindow();

    input.forward  = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    input.backward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    input.left     = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    input.right    = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    if (mFirstMouse)
    {
        mLastMouseX = x;
        mLastMouseY = y;
        mFirstMouse = false;
    }

    input.mouseDeltaX = static_cast<float>(x - mLastMouseX);
    input.mouseDeltaY = static_cast<float>(mLastMouseY - y);

    mLastMouseX = x;
    mLastMouseY = y;

    return input;
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
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::EndFrame()
{
    mWindow.SwapBuffers();
    mWindow.PollEvents();
}

int Engine::CreateMesh(const float* vertices, size_t vertexFloatCount, const uint32_t* indices, size_t indexCount)
{
    Mesh mesh{};

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);
    
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertexFloatCount * sizeof(float),
        vertices,
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &mesh.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, 
        indexCount * sizeof(uint32_t),
        indices, 
        GL_STATIC_DRAW
    );

    constexpr GLsizei stride = 11 * sizeof(float);

    // position
    glVertexAttribPointer(
        0, 
        3, 
        GL_FLOAT, 
        GL_FALSE, 
        stride, 
        (void*)0
    );
    glEnableVertexAttribArray(0);

    // uv map
    glVertexAttribPointer(
        1, 
        2, 
        GL_FLOAT, 
        GL_FALSE, 
        stride, 
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // normals
    glVertexAttribPointer(
        2, 
        3, 
        GL_FLOAT, 
        GL_FALSE, 
        stride, 
        (void*)(5 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    // tangents
    glVertexAttribPointer(
        3, 
        3, 
        GL_FLOAT, 
        GL_FALSE, 
        stride, 
        (void*)(8 * sizeof(float))
    );
    glEnableVertexAttribArray(3);

    mesh.indexCount = static_cast<GLsizei>(indexCount);

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

GLFWwindow* Engine::GetNativeWindow() const
{
    return mWindow.GetNativeWindow();
}

void Engine::HideMouse()
{
    glfwSetInputMode(GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Engine::ShowMouse()
{
    glfwSetInputMode(GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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