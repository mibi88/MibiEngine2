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

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

#define EXIT(rc) {free_on_exit(); exit(rc);}

unsigned long get_ms(void) {
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return time.tv_nsec/(1e6)+time.tv_sec*1000;
}

GEShaderPos projection_mat_pos, view_mat_pos, model_mat_pos;
GEShaderPos normal_mat_pos;
GEShaderPos uv_max_pos;
GEShaderPos model_tex_pos;

GEShaderPos fb_size_pos;

GEMat4 projection_mat, view_mat, model_mat;
GEMat3 normal_mat;

unsigned long last_time = 0;

GEWindow window;
GEObj obj;
GEShader shader;
GEShader fb_shader;
GEModel model;

GEImage image;
GETexture texture;

GEFramebuffer framebuffer;

GEStdShader stdshader;

GEScene scene;
GERenderable renderable;
GEEntity entity;

GEEntity *scene_entity;

/* TODO: Fix memory leaks */

void free_on_exit(void) {
    puts("Free everything!");
    ge_scene_free(&scene);
    ge_renderable_free(&renderable);
    ge_obj_free(&obj);
    ge_texture_free(&texture);
    ge_image_free(&image);
    ge_shader_free(&shader);
    ge_shader_free(&fb_shader);
    ge_stdshader_free(&stdshader);
    ge_framebuffer_free(&framebuffer);
    ge_window_free(&window);
    puts("Successfully freed everything!");
}

void init(void) {
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
    char *model_attr_names[] = {
        "vertex",
        "color",
        "uv",
        "normal"
    };
    char *log;
    last_time = get_ms();
    
    if((log = ge_loader_load_shader(&shader, "shaders/vertex_3d.vert",
                                    "shaders/fragment_3d.frag")) != NULL){
        puts(log);
        fputs("Failed to load shader!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if((log = ge_loader_load_shader(&fb_shader, "shaders/vertex_fb.vert",
                                    "shaders/fragment_fb.frag")) != NULL){
        puts(log);
        fputs("Failed to load shader!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_stdshader_init(&stdshader, &shader)){
        fputs("Failed to create stdshader!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    projection_mat_pos = ge_shader_get_pos(&shader, "ge_projection_mat");
    view_mat_pos = ge_shader_get_pos(&shader, "ge_view_mat");
    model_mat_pos = ge_shader_get_pos(&shader, "ge_model_mat");
    normal_mat_pos = ge_shader_get_pos(&shader, "ge_normal_mat");
    uv_max_pos = ge_shader_get_pos(&shader, "uv_max");
    model_tex_pos = ge_shader_get_pos(&shader, "tex");
    
    fb_size_pos = ge_shader_get_pos(&fb_shader, "size");
    
    ge_mat4_identity(&view_mat);
    
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
    
    ge_shader_use(&shader);
    
    if(ge_image_init(&image, "spot_texture.png")){
        fputs("Failed to read image!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_texture_init(&texture, &image, 1, 0)){
        fputs("Failed to load texture!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    if(ge_loader_load_obj(&model, &shader, &texture, "spot.obj",
                          model_attr_names, "tex")){
        fputs("Failed to load model!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    if(ge_loader_model_renderable(&renderable, &model, &stdshader)){
        fputs("Failed to create renderable!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    if(ge_entity_init(&entity, &renderable)){
        fputs("Failed to create entity!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    if(ge_scene_init(&scene, &entity, 1, 0)){
        fputs("Failed to create scene!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if((scene_entity = ge_scene_get_same_entity(&scene, &entity)) == NULL){
        fputs("Failed to get entity!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
}

float x = 0;

void draw(void *data) {
    float delta;
    (void)data;
    
    /* Render everything */
    ge_shader_use(&shader);
#if POSTPROCESSING
    ge_framebuffer_use(&framebuffer);
#endif
    ge_window_clear(&window, 0, 0, 0, 1);
    
#if PRINT_MS
    printf("%ld      \r", get_ms()-last_time);
#endif
    delta = (get_ms()-last_time)*0.001;
    last_time = get_ms();
    
    ge_entity_set_position(scene_entity, 0, 0, -2.2);
    ge_entity_set_rotation(scene_entity, 0, x, 0);
    ge_entity_update(scene_entity);
    
    ge_shader_load_mat4(&projection_mat_pos, &projection_mat);
    ge_shader_load_mat4(&view_mat_pos, &view_mat);
    ge_shader_load_vec2(&uv_max_pos, &texture.uv_max);
    
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
    ge_mat4_projection3d(&projection_mat, 72, w/(float)h, 1000, 1);
    if(ge_framebuffer_resize(&framebuffer, w, h)){
        fputs("Failed to resize framebuffer!\n", stderr);
    }
    printf("Window resized to %dx%d\n", w, h);
}

int main(int argc, char **argv) {
    int rc;
    (void)argc;
    (void)argv;
    
    if((rc = ge_window_init(&window, "MibiEngine2 demo"))){
        return rc;
    }
    
    init();
    
    if(ge_window_set_callbacks(&window, draw, resize)){
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

