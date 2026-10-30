#include "renderer/engine.h"

int main()
{
    Engine engine;
    engine.init(1280, 720);

    while (!engine.should_close()) {
        engine.begin_frame();

        engine.end_frame();
    }

    engine.cleanup();

    return 0;
}