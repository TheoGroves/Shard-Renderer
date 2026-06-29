#include <iostream>
#include <string>
#include <string_view>
#include <format>
#include "Core/Engine.h"
#include "Core/Camera.h"
#include "Math/Vec3.h"
#include "Testing/PlayerController.h"

const char* title = "Shard Engine";
constexpr unsigned int screenWidth = 1920;
constexpr unsigned int screenHeight = 1080;

int main(void)
{
    Engine engine;
    Camera cam;
    PlayerController controller;

    if (!engine.Initialize(screenWidth, screenHeight, title))
        return -1;

    int triangle = engine.CreateMesh();

    glfwSetInputMode(engine.GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!engine.ShouldClose())
    {
        float dt = 0.016f;
        controller.Update(cam, engine.GetNativeWindow(), dt);

        cam.forward = ForwardFromEuler(cam.rotation);

        engine.UpdateMat4("uModel", Mat4::Identity());
        engine.UpdateMat4("uView", cam.GetView());
        engine.UpdateMat4("uProj", cam.GetProjection());

        engine.BeginFrame();

        engine.DrawMesh(triangle);

        engine.EndFrame();
    }

    engine.Shutdown();
    return 0;
}