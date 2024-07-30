**Morningstar is _NOT_ currently in a finished state. Many, many things do not work or are straight up unimplemented. Come back when its filled out more.**

## The Morningstar Toolkit
This project is more-or-less a minimal game framework for 2D pixelart RPG-style games. It provides development utilities like debug and error logging, game-related objects like windows, renderers, and localization tools. **But**, this toolkit is not meant for use outside of the types of games I like to create. Without significant source code editing on your side, the UI is static, most processes are static, the rendering loop is static. This is for the sake of performance and my ease of development, but it limits the use case of the project.

### Dependencies
Morningstar is a fairly minimal project, and that's by design. I want it to be performant and small. Now, that being said, it does require two really, really big dependencies; a Linux distribution and a Wayland desktop environment. These are the only two dependencies fully wound into the source code, but the full list is below.

- GNU/Linux (linux) - I use some optimizations, key codes, functions, and headers that only exist on Linux.
- Wayland (libwayland) - I directly interface with Wayland when rendering.
    - XDG-shell (libwayland-protocols) - I use the XDG window manager protocol alongside Wayland for ease of, well, window management.
- CMake - I use CMake as a build system, but it's fairly trivial to convert it to a Makefile or Meson system or what have you.

### Building / Installation
Building the framework is fairly easy, and should be completely painless. If you run into any issues that are on my side, not yours, make sure to open an [Issue](https://github.com/israfiel-a/morningstar/issues/new)! A Bash script to build the library is below:

```bash
# Assuming you're already in the Morningstar folder that you've
# either downloaded from Github or `git clone`d.
cd Program
cmake -B build . && cd build
make
cd Morningstar
ls

# Should output (or at least something similar):
# . .. libmorningstar.so
```

There is also a secondary debug mode that you can compile for, in which CMake will look for a test C file. An exmaple of a test file is below:

```c
// Main.c - example test file
#include <Diagnostic/Time.h>  // The application clock.
#include <Windowing/Client.h> // The Wayland client file.
#include <Input/Terminal.h>   // Commandline functionality.
#include <Globals.h>          // Application global flags.

int main(int argc, char** argv)
{
    (void)GetCurrentTime(); // Start the clock.
    // Check if we're running within a terminal.
    if (isatty(STDOUT_FILENO)) global_flags.stdout_available = true;
    // Handle any command line arguments.
    HandleCommandLineArgs(argc, argv);

    // Setup Wayland, run the application, and then destroy it.
    SetupWayland();
    DestroyWayland();
    return 0; // Return success.
}
```

And the way to trigger the debug mode would be:

```bash
# Assuming you're already in the Morningstar folder that you've
# either downloaded from Github or `git clone`d.
cd Program
cmake -B build -DCMAKE_BUILD_TYPE=Debug && cd build
make
cd Morningstar
ls

# Should output (or at least something similar):
# . .. libmorningstar.so Morningstar
```