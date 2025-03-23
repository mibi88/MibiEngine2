                               MIBIENGINE2
                                by Mibi88

My third game engine actually. It is made to be easy to add other backends to
support a lot of different rendering APIs.

It is currently in a very early state, it isn't really usable yet, it will
still change a lot, and it has very few features.

TODO:
[ ] Entities and scenes for easier usage.
[ ] Make correct lighting, support multiple lights and load them to the shader
    as uniforms when rendering a scene.
[ ] Postprocessing
    [ ] Bloom
    [ ] Ambient occlusion
[ ] Create a universal shader language that will work on any backend (by
    converting it to the backend specific shader language when loading the
    shaders).
[ ] Write a custom utility to parse the documentation in the header files.

