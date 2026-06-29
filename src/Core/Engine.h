#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include "Rendering/Window.h"
#include "Rendering/Shader.h"
#include "Math/Mat4.h"

class Engine
{
public:
    static void LogMessage(std::string_view message);
    static void LogWarning(std::string_view warning);
    static void LogError(std::string_view error);

    void UpdateMat4(const std::string& uniform, const Mat4& matrix);

    bool Initialize(unsigned int screenWidth, unsigned int screenHeight, std::string title);
    
    void BeginFrame();
    void EndFrame();

    int CreateMesh();
    void DrawMesh(int meshID);

    GLFWwindow* GetNativeWindow() const;

    void Shutdown();

    bool ShouldClose() const;
private:
    static void GLFWErrorCallback(int error, const char* description);
    static void APIENTRY GLDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

    Window mWindow;
    bool mRunning = true;

    Shader mShader;

    struct Mesh
    {
        GLuint vao = 0;
        GLuint vbo = 0;
        GLuint ebo = 0;
        int indexCount = 0;
    };

    std::unordered_map<int, Mesh> mMeshes;
    int mNextMeshID = 1;
};