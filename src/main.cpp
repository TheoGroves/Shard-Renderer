#include "renderer/engine.h"

int main()
{
    Engine engine(1280, 720);

    while (!engine.should_close()) {
        engine.begin_frame();

        engine.end_frame();
    }

    return 0;
}