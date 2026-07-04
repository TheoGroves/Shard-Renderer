#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include "Rendering/Window.h"
#include "Rendering/Shader.h"
#include "Math/Mat4.h"
#include "Core/Input.h"

class Engine
{
public:
    static void LogMessage(std::string_view message);
    static void LogWarning(std::string_view warning);
    static void LogError(std::string_view error);

    GLuint CreateTextureRGBA(int width, int height, const std::vector<uint8_t>& pixels);
    GLuint CreateTextureRGB32F(int width, int height, const std::vector<float>& pixels);

    void UpdateInt(const std::string& uniform, int value);
    void UpdateMat4(const std::string& uniform, const Mat4& matrix);

    void BindTexture(GLuint texture, GLuint unit);

    Input GetInput();

    bool Initialize(unsigned int screenWidth, unsigned int screenHeight, std::string title);
    
    void BeginFrame();
    void EndFrame();

    int CreateMesh(const float* vertices, size_t vertexFloatCount, const uint32_t* indices, size_t indexCount);
    void DrawMesh(int meshID);

    GLFWwindow* GetNativeWindow() const;

    void HideMouse();
    void ShowMouse();

    void Shutdown();

    bool ShouldClose() const;
private:
    static void GLFWErrorCallback(int error, const char* description);
    static void APIENTRY GLDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

    double mLastMouseX = 0.0;
    double mLastMouseY = 0.0;
    bool mFirstMouse = true;

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