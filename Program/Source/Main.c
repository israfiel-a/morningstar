#include <Windowing/Windowing.h>

int main(int argc, char** argv)
{
    SetupWindow(false);
    CreateWindow(TITLE);

    run();

    DestroyWindow();
}
