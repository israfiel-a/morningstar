#include <Rendering/Loop.h>
#include <Windowing/Windowing.h>

int main(int argc, char** argv)
{
    SetupWindow();
    CreateRenderingThread();

    CreateWindow(TITLE);
    panel_t* backdrop = CreatePanel(center_filler);
    if (backdrop == NULL) return 9;

    run();

    DestroyWindow();
}
