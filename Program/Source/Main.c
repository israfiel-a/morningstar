#include <Windowing/Windowing.h>

int main(int argc, char** argv)
{
    SetupWindow();
    CreateWindow(TITLE);

    run();

    DestroyWindow();
}
