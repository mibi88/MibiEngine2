                               MIBIENGINE2
                                by Mibi88

MibiEngine2 is a small game engine meant to be very efficient (I'm coding it on
a RPi 4), portable, by supporting multiple backends (currently it only has a
backend for OpenGL ES 2) and being written in ANSI C with minimal dependencies.

It actually is my third game engine.

My first engine was MibiFlightSimulator's engine, that uses modern OpenGL.

First of all, it was written in Java, but I'm way more familiar with ANSI C.
The code also wasn't so clean, I named all my variables in sneak case even if
in Java variable names should be written in camel case.
It also wasn't flexible enough which made it painful to improve, that's
something I'm trying to do better here.

MibiEngine was my second game engine, which was targetting 20 years old
computers. I had built it using MibiCraft's rendering code, built a GUI toolkit
on top of it and tried to make a custom build system for it. It also has a GUI
to handle building projects, but it has a lot of memory leaks, the code is very
complicated and messy, and I'm having issues communicating between the process
that handles building and the other where the GUI runs on. I never finished and
released it publicly. I'm coding in my free time and I'm very young, so I don't
have a lot of time to code, I'm coding a little bit every day and it makes it
hard to finish a project with clean code. I may finish it at some point, it may
be interesting.

I may also add support later for legacy OpenGL in MibiEngine2, it may be cool,
but I don't want to make the code messy, so I'll have to find a clean solution
to do it if I want to.

It is currently in a very early state, it isn't really usable yet, it will
still change a lot, and it has very few features.

    DEPENDENCIES

 - A C compiler able to compile ANSI C (that shouldn't be a problem XD)
 - C standard library (including the standard math library)
 - libpng
For the OpenGL ES backend:
 - Xlib
 - EGL
 - OpenGL ES 2

    BUILDING

Currently, only UNIX-like operating systems are supported (I'm only testing it
on Linux).

In a terminal emulator, run

$ ./build.sh

It should compile the engine and a sample program, which can be launched as
following

$ ./main

All the documentation is in the header files in include/mibiengine2

    TODO

[ ] Entities and scenes for easier usage.
[ ] Make correct lighting, support multiple lights and load them to the shader
    as uniforms when rendering a scene.
[ ] Postprocessing
    [ ] Bloom
    [ ] Ambient occlusion
[ ] Create a universal shading language that will work on any backend (by
    converting it to the backend specific shading language when loading the
    shaders).
[ ] Write a custom utility to parse the documentation in the header files.
[ ] Write a custom build system
[ ] Code a GUI toolkit
[ ] Code a scene editor
[ ] Code a scripting language

    LICENSE

This project is licensed under the BSD-3-Clause license (see COPYING).

