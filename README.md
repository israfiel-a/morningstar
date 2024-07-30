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
Building the framework is fairly easy, and should be completely painless.