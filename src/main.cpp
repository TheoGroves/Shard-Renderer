#include "renderer/engine.h"
#include <chrono>
#include <iostream>

int main()
{
    int fps_max = 0;

    Engine engine(1280, 720);

    while (!engine.should_close()) {
        engine.begin_frame();
        engine.end_frame();
    }

    return 0;
}