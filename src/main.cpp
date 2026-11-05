#include "renderer/engine.h"
#include <chrono>
#include <iostream>

int main()
{
    int fps_max = 0;

    Engine engine(1280, 720);

    while (!engine.should_close()) {
        auto start = std::chrono::steady_clock::now();

        engine.begin_frame();
        engine.end_frame();

        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> dt = end - start;
        double fps = (dt.count() > 0.0) ? (1.0 / dt.count()) : 0.0;
        int int_fps = static_cast<int>(fps);

        if (int_fps > fps_max) {
            fps_max = int_fps;
        }
    }

    std::cout << "\nMax FPS: " << fps_max << '\n';

    return 0;
}