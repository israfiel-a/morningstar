**Morningstar is _NOT_ currently in a finished state. Many, many things do not work or are straight up unimplemented (like Windows functionality). Come back when it's filled out more.**

## The Morningstar Toolkit
This project is more-or-less a minimal game framework for 2D pixelart RPG-style games. It provides development utilities like debug and error logging, game-related objects like windows, renderers, and localization tools. **But**, this toolkit is not meant for use outside of the types of games I like to create. Without significant source code editing on your side, the UI is static, most processes are static, the rendering loop is static. This is for the sake of performance and my ease of development, but it limits the use case of the project.

### Dependencies
Morningstar is a fairly minimal project, and that's by design. I want it to be performant and small. Now, that being said, it does require more than one dependency. I'm not a wizard.

- **Windowing**
    - Wayland (libwayland) - If you're on Linux, this app does not interface with X11. Only Wayland. Your Wayland compositor must also be fairly up to date (as in, updated in the last two years or so).
        - XDG-shell (libwayland-protocols) - I use the XDG window manager protocol alongside Wayland for actual window creation and management.
    - Windows (win32api) - If you're on Windows, then this is all you need windowing-wise, and it comes with every distribution of Windows since...[1985](https://en.wikipedia.org/wiki/Windows_API) (although I'm sure a 1985 copy of Windows wouldn't exactly handle this program very well).
- **Rendering**
    - Vulkan (libvulkan) - I use Vulkan as my rendering API within the toolkit. I forgo OpenGL support since, these days, nearly every home computer comes with a distribution of this, and if it doesn't, it's easily installable.
        - Vulkan Wayland Extension (vulkan_wayland.h) - If you're on the Wayland platform, the Vulkan Wayland extension is necessary to force Vulkan to play nice with our window. Windows users don't have to worry about this.
    - STBI (stbi.h) - STBI is a must for me for image processing. I understand that the process behind it really isn't that complicated once sufficient understanding of compression algorithims is achieved, but I'm lazy. And it's a single header (well, I separate it into a header and separate static library file to link into the main application...but, well, that isn't important). This dependency comes packaged with the toolkit, so don't bother worrying about it.
- **Building**
    - CMake - I use CMake as a build system, but it's fairly trivial to convert it to a Makefile or Meson system or what have you.

### Usage
Using the Morningstar toolkit should be just as painless as building it. All documentation is contained within the header files copied into the build folder during compilation. Literally all you have to do is a set of three steps.

1. Build the framework. If you run into any issues that are on my side, not yours, make sure to open an [Issue](https://github.com/israfiel-a/morningstar/issues/new)! A Bash script to build the library is below:

    ```bash
    # Assuming you're already in the Morningstar folder that you've
    # either downloaded from Github or `git clone`d.
    cd Program
    cmake -B build . && cd build
    make
    cd Morningstar

    # Should output a build/Morningstar folder with the structure
    # . .. libmorningstar_shared.so libmorningstar_static.a [morningstar headers].h
    ```

    There is also a secondary debug mode that you can build for, in which CMake will look for a test C file. One is included with the project. To trigger debug mode, simply add a flag to the CMake call:

    ```bash
    # ...
    cmake -B build -DCMAKE_BUILD_TYPE=Debug && cd build
    # ...

    # Should output a build/Morningstar folder with the structure
    # . .. libmorningstar_shared.so libmorningstar_static.a MorningstarDemo
    ```

2. Copy the library files into your project's dependency folder. All library files are within the `build/Morningstar` folder.

3. Include the files as you need, and link the toolkit during compilation. Static or shared doesn't particularly matter for a library so small, but I suggest the shared object file. Do note that when linking with a shared object file, you should make sure the executable either stays in one place or is linked with something like `CMAKE_BUILD_RPATH "$ORIGIN/"`.