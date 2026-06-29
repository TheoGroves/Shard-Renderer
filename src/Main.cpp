#include <iostream>
#include <string>
#include <string_view>
#include <format>
#include "Engine.h"

const char* title = "Shard Engine";
constexpr unsigned int screenWidth = 1920;
constexpr unsigned int screenHeight = 1080;

int main(void)
{
    Engine engine;

    if (!engine.Initialize(screenWidth, screenHeight, title))
        return -1;

    int triangle = engine.CreateMesh();

    while (!engine.ShouldClose())
    {
        engine.BeginFrame();

        engine.DrawMesh(triangle);

        engine.EndFrame();
    }

    engine.Shutdown();
    return 0;
}