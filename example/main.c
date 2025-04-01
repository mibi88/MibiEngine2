/* A small OpenGL ES engine.
 * by Mibi88
 *
 * This software is licensed under the BSD-3-Clause license:
 *
 * Copyright 2025 Mibi88
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mibiengine2/base/mat.h>
#include <mibiengine2/base/window.h>
#include <mibiengine2/base/texturedmodel.h>

#include <mibiengine2/base/shader.h>
#include <mibiengine2/base/config.h>

#include <mibiengine2/base/image.h>
#include <mibiengine2/base/texture.h>

#include <mibiengine2/base/obj.h>

#include <mibiengine2/base/framebuffer.h>

#include <mibiengine2/renderer/loader.h>
#include <mibiengine2/renderer/scene.h>
#include <mibiengine2/renderer/stdshader.h>

#define PRINT_MS 1

#define POSTPROCESSING 1

#define ENTITIES 10

#define EXIT(rc) {free_on_exit(); exit(rc);}

GEShaderPos fb_size_pos;

unsigned long last_time = 0;

GEWindow window;
GEShader shader;
#if POSTPROCESSING
GEShader fb_shader;
#endif

GEImage image;
GETexture texture;

#if POSTPROCESSING
GEFramebuffer framebuffer;
#endif

GEStdShader stdshader;

GEScene scene;
GERenderable renderable;
GEEntity entities[ENTITIES];

GECamera camera;

GEStdShader *scene_shaders[] = {
    &stdshader
};

void free_on_exit(void) {
    puts("Free everything!");
    ge_scene_free(&scene);
    ge_renderable_free(&renderable);
    ge_texture_free(&texture);
    ge_image_free(&image);
    ge_shader_free(&shader);
#if POSTPROCESSING
    ge_shader_free(&fb_shader);
    ge_framebuffer_free(&framebuffer);
#endif
    ge_stdshader_free(&stdshader);
    ge_camera_free(&camera);
    ge_window_free(&window);
    puts("Successfully freed everything!");
}

unsigned int seed = 77;

unsigned int xorshift(unsigned int *seed) {
    /* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
    *seed ^= *seed << 13;
    *seed ^= *seed >> 17;
    *seed ^= *seed << 5;
    return *seed;
}

void keypress(void *data, int key, int released) {
    (void)data;
    if(released){
        printf("Key %d released\n", key);
    }else{
        printf("Key %d pressed\n", key);
    }
}

void init(void) {
    size_t i;
#if POSTPROCESSING
    GEColor colors[2] = {GE_C_RGBA, GE_C_RGBA};
    GETexType tex_types[2] = {GE_TEX_COLOR, GE_TEX_DEPTH};
    char linear[2] = {1, 0};
    char *fb_attr_names[] = {
        "vertex",
        NULL,
        "uv",
        NULL
    };
    char *tex_names[] = {
        "color",
        "depth"
    };
#endif
    char *log;
    last_time = ge_window_ms(&window);
    
    if((log = ge_loader_load_shader(&shader, "shaders/vertex_3d.vert",
                                    "shaders/fragment_3d.frag")) != NULL){
        puts(log);
        fputs("Failed to load shader!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    if(ge_stdshader_init(&stdshader, &shader)){
        fputs("Failed to create stdshader!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    ge_shader_use(&shader);
    
    if(ge_image_init(&image, "spot_texture.png")){
        fputs("Failed to read image!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_texture_init(&texture, &image, 1, 0)){
        fputs("Failed to load texture!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    if(ge_loader_load_obj_as_renderable(&renderable, &stdshader, &texture,
                                        "spot.obj")){
        fputs("Failed to load model as renderable!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    for(i=0;i<ENTITIES;i++){
        if(ge_entity_init(entities+i, &renderable)){
            fputs("Failed to create entity!\n", stderr);
            EXIT(EXIT_FAILURE);
        }
        ge_entity_set_position(entities+i,
                               (xorshift(&seed)%1024)/(float)1024*4-2,
                               (xorshift(&seed)%1024)/(float)1024*2-1,
                               -2.2-((xorshift(&seed)%256)/(float)256)*16);
        ge_entity_set_scale(entities+i, 0.5, 0.5, 0.5);
        ge_entity_update(entities+i);
    }
    
    if(ge_scene_init(&scene, entities, ENTITIES, scene_shaders, 1, 0)){
        fputs("Failed to create scene!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_camera_init(&camera)){
        fputs("Failed to init camera!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    ge_camera_perspective(&camera, 72, 480/(float)360, 1000, 0.1);
    ge_scene_set_camera(&scene, &camera);
    
#if POSTPROCESSING
    if((log = ge_loader_load_shader(&fb_shader, "shaders/vertex_fb.vert",
                                    "shaders/fragment_fb.frag")) != NULL){
        puts(log);
        fputs("Failed to load shader!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    fb_size_pos = ge_shader_get_pos(&fb_shader, "size");
    
    ge_shader_use(&fb_shader);
    
    if(ge_framebuffer_init(&framebuffer, 480, 360, 2, colors, tex_types,
                           linear)){
        fputs("Failed to initialize framebuffer!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_framebuffer_attr(&framebuffer, &fb_shader, fb_attr_names, tex_names,
                           &fb_size_pos)){
        fputs("Failed to initialize framebuffer attr!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
#endif
}

float x = 0;

void rotate_entities(GEEntity *entity, void *data) {
    (void)data;
    ge_entity_set_rotation(entity, 0, x, 0);
    ge_entity_update(entity);
}

void draw(void *data) {
    float delta;
    unsigned long now, diff;
    (void)data;
    
    /* Render everything */
    ge_shader_use(&shader);
#if POSTPROCESSING
    ge_framebuffer_use(&framebuffer);
#endif
    ge_window_clear(&window, 0, 0, 0, 1);
    
    now = ge_window_ms(&window);
    diff = now-last_time;
    delta = diff*0.001;
    last_time = now;
    
#if PRINT_MS
    printf("%lu      \r", diff);
#endif
    
    if(ge_window_key_pressed(&window, GE_K_UP)){
        camera.position.z += 2*delta;
    }
    if(ge_window_key_pressed(&window, GE_K_DOWN)){
        camera.position.z -= 2*delta;
    }
    /*if(ge_window_key_pressed(&window, GE_K_LEFT)){
        camera.rotation.y += delta;
    }
    if(ge_window_key_pressed(&window, GE_K_RIGHT)){
        camera.rotation.y -= delta;
    }*/
    ge_camera_update(&camera);
    
    ge_scene_for_entity(&scene, rotate_entities, NULL);
    
    ge_scene_update(&scene);
    
    ge_scene_render(&scene);
    
#if POSTPROCESSING
    ge_framebuffer_default();
    ge_shader_use(&fb_shader);
    
    ge_framebuffer_render(&framebuffer);
#endif
    
    x += 2.5*delta;
    while(x > 360) x -= 360;
}

void resize(void *data, int w, int h) {
    (void)data;
    ge_window_view(&window, w, h);
    ge_camera_perspective(&camera, 72, w/(float)h, 1000, 0.1);
#if POSTPROCESSING
    if(ge_framebuffer_resize(&framebuffer, w, h)){
        fputs("Failed to resize framebuffer!\n", stderr);
    }
#endif
    printf("Window resized to %dx%d\n", w, h);
}

int main(int argc, char **argv) {
    int rc;
    (void)argc;
    (void)argv;
    
    if((rc = ge_window_init(&window, "MibiEngine2 demo"))){
        return rc;
    }
    
    ge_window_cap_framerate(&window, 1);
    
    init();
    
    if(ge_window_set_callbacks(&window, draw, resize, keypress)){
        fputs("Failed to set callbacks!\n", stderr);
        free_on_exit();
        return EXIT_FAILURE;
    }
    if(ge_window_set_data(&window, &window)){
        fputs("Failed to set data!\n", stderr);
        free_on_exit();
        return EXIT_FAILURE;
    }
    ge_window_mainloop(&window);
    
    free_on_exit();
    
    return EXIT_SUCCESS;
}

