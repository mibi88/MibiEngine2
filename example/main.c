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
#include <math.h>

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

#include <mibiengine2/render2d/sprite.h>
#include <mibiengine2/render2d/font.h>
#include <mibiengine2/render2d/text.h>
#include <mibiengine2/render2d/tilemap.h>

#define PRINT_MS 1

#define POSTPROCESSING 1

#define ENTITIES 40

#define EXIT(rc) {free_on_exit(); exit(rc);}

GEShaderPos fb_size_pos;

unsigned long last_time = 0;

GEWindow window;
GEShader shader3d;
GEShader shader2d;
#if POSTPROCESSING
GEShader fb_shader;
#endif

GEImage image;
GETexture texture;

#if POSTPROCESSING
GEFramebuffer framebuffer;
#endif

GEStdShader stdshader3d;
GEStdShader stdshader2d;

GEScene scene3d;
GERenderable renderable;
GEEntity entities[ENTITIES];

GEScene scene2d;

GECamera camera3d;
GECamera camera2d;

GESprite sprite;
GERenderable sprite_renderable;

GEImage font_image;
GETexture font_texture;
GEFont font;
GEText text;
GERenderable text_renderable;

GEImage tileset;
GETexture tileset_texture;
GETilemap tilemap;
GERenderable tilemap_renderable;
unsigned short int tiles[16*16] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 1,
    1, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1,
    1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 1,
    1, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1,
    1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 1,
    1, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1,
    1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 1,
    1, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1,
    1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 1,
    1, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1,
    1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 1,
    1, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1,
    1, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 1,
    1, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

GEEntity entities2d[3];

GEEntity *sprite_entity = entities2d;
GEEntity *text_entity = entities2d+2;
GEEntity *tilemap_entity = entities2d+1;

GEStdShader *scene3d_shaders[] = {
    &stdshader3d
};
GEStdShader *scene2d_shaders[] = {
    &stdshader2d
};

void free_on_exit(void) {
    puts("Free everything!");
    ge_scene_free(&scene3d);
    ge_scene_free(&scene2d);
    ge_renderable_free(&renderable);
    ge_renderable_free(&sprite_renderable);
    ge_renderable_free(&text_renderable);
    ge_renderable_free(&tilemap_renderable);
    ge_font_free(&font);
    ge_text_free(&text);
    ge_tilemap_free(&tilemap);
    ge_texture_free(&texture);
    ge_texture_free(&font_texture);
    ge_texture_free(&tileset_texture);
    ge_image_free(&image);
    ge_image_free(&font_image);
    ge_image_free(&tileset);
    ge_shader_free(&shader3d);
    ge_shader_free(&shader2d);
#if POSTPROCESSING
    ge_shader_free(&fb_shader);
    ge_framebuffer_free(&framebuffer);
#endif
    ge_stdshader_free(&stdshader3d);
    ge_camera_free(&camera3d);
    ge_camera_free(&camera2d);
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

void mouseevent(void *data, int x, int y, GEWindowMouseEvent event,
                int released) {
    (void)data;
    printf("Mouse event: x: %d, y: %d, event: %d, released: %d\n", x, y,
           event, released);
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
    
    if((log = ge_loader_load_shader(&shader3d, "shaders/vertex_3d.vert",
                                    "shaders/fragment_3d.frag")) != NULL){
        puts(log);
        fputs("Failed to load shader!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_stdshader_init(&stdshader3d, &shader3d)){
        fputs("Failed to create stdshader!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    if((log = ge_loader_load_shader(&shader2d, "shaders/vertex_2d.vert",
                                    "shaders/fragment_2d.frag")) != NULL){
        puts(log);
        fputs("Failed to load shader!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_stdshader_init(&stdshader2d, &shader2d)){
        fputs("Failed to create stdshader!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    if(ge_image_init(&image, "spot_texture.png")){
        fputs("Failed to read image!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_texture_init(&texture, &image, 1, 0)){
        fputs("Failed to load texture!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    ge_shader_use(&shader2d);
    
    if(ge_sprite_init(&sprite, &texture, &stdshader2d, 64, 64)){
        fputs("Failed to create sprite!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_loader_model_renderable(&sprite_renderable,
                                  &GE_SPRITE_MODEL(&sprite),
                                  &stdshader2d)){
        fputs("Failed to create sprite renderable!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_entity_init(sprite_entity, &sprite_renderable)){
        fputs("Failed to create sprite entity!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    if(ge_image_init(&font_image, "font.png")){
        fputs("Failed to read image!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_texture_init(&font_texture, &font_image, 0, 0)){
        fputs("Failed to load texture!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_font_init(&font, &font_image, 0, 3, 0, 1, 1, GE_F_ASCII)){
        fputs("Failed to create font!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_text_init(&text, &font, &font_texture, &stdshader2d, "Hello world!",
                    6)){
        fputs("Failed to create text!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_text_update(&text, "The text got updated!")){
        fputs("Failed to update text!\n", stderr);
    }
    if(ge_loader_model_renderable(&text_renderable,
                                  &GE_TEXT_GET_MODEL(&text),
                                  &stdshader2d)){
        fputs("Failed to create text renderable!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_entity_init(text_entity, &text_renderable)){
        fputs("Failed to create text entity!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    if(ge_image_init(&tileset, "tiles.png")){
        fputs("Failed to read image!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_texture_init(&tileset_texture, &tileset, 0, 0)){
        fputs("Failed to load texture!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_tilemap_init(&tilemap, &tileset, &tileset_texture, &stdshader2d,
                       tiles, 16, 16, 4, 1, 32)){
        fputs("Failed to create tilemap!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    tiles[3*16+5] = 3;
    if(ge_tilemap_update(&tilemap, tiles, 16, 16, 32)){
        fputs("Failed to update the tilemap!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_loader_model_renderable(&tilemap_renderable,
                                  &GE_TILEMAP_GET_MODEL(&tilemap),
                                  &stdshader2d)){
        fputs("Failed to create tilemap renderable!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_entity_init(tilemap_entity, &tilemap_renderable)){
        fputs("Failed to create tilemap entity!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    ge_shader_use(&shader3d);
    
    if(ge_loader_load_obj_as_renderable(&renderable, &stdshader3d, &texture,
                                        "spot.obj", 0)){
        fputs("Failed to load model as renderable!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    for(i=0;i<ENTITIES;i++){
        if(ge_entity_init(entities+i, &renderable)){
            fputs("Failed to create entity!\n", stderr);
            EXIT(EXIT_FAILURE);
        }
        ge_entity_set_position(entities+i,
                               (xorshift(&seed)%1024)/(float)1024*16-8,
                               (xorshift(&seed)%1024)/(float)1024*2-1,
                               ((xorshift(&seed)%256)/(float)256)*16-8);
        ge_entity_set_scale(entities+i, 0.5, 0.5, 0.5);
        ge_entity_update(entities+i);
    }
    
    if(ge_scene_init(&scene3d, entities, ENTITIES, scene3d_shaders, 1, 0)){
        fputs("Failed to create scene!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_scene_init(&scene2d, entities2d, 3, scene2d_shaders, 1, 0)){
        fputs("Failed to create 2D scene!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_camera_init(&camera3d)){
        fputs("Failed to init 3D camera!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    ge_camera_perspective(&camera3d, 72, 480/(float)360, 1000, 0.1);
    ge_scene_set_camera(&scene3d, &camera3d);
    if(ge_camera_init(&camera2d)){
        fputs("Failed to init 2D camera!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    GE_CAMERA_ORTHO2D(&camera2d, 0, 0, 640, 480);
    ge_scene_set_camera(&scene2d, &camera2d);
    
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

void move_sprites(GEEntity *entity, void *data) {
    (void)data;
    /*ge_entity_set_rotation(entity, 0, 0, x);*/
    ge_entity_set_position(entity, -x*1.5, -x*1.8, 0);
    ge_entity_update(entity);
}

void draw(void *data) {
    float delta;
    unsigned long now, diff;
    (void)data;
    
    /* Render everything */
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
        camera3d.position.x += 15*cos(-camera3d.rotation.y/180*GE_MAT_PI+
                               (GE_MAT_PI/2))*delta;
        camera3d.position.z += 15*sin(-camera3d.rotation.y/180*GE_MAT_PI+
                               (GE_MAT_PI/2))*delta;
    }
    if(ge_window_key_pressed(&window, GE_K_DOWN)){
        camera3d.position.x -= 15*cos(-camera3d.rotation.y/180*GE_MAT_PI+
                               (GE_MAT_PI/2))*delta;
        camera3d.position.z -= 15*sin(-camera3d.rotation.y/180*GE_MAT_PI+
                               (GE_MAT_PI/2))*delta;
    }
    if(ge_window_key_pressed(&window, GE_K_LEFT)){
        camera3d.rotation.y += 180*delta;
    }
    if(ge_window_key_pressed(&window, GE_K_RIGHT)){
        camera3d.rotation.y -= 180*delta;
    }
    ge_camera_update(&camera3d);
    
    ge_scene_for_entity(&scene3d, rotate_entities, NULL);
    ge_scene_for_entity(&scene2d, move_sprites, NULL);
    
    ge_scene_update(&scene3d);
    ge_scene_update(&scene2d);
    
    ge_shader_use(&shader3d);
    
    ge_scene_render(&scene3d);
    
    ge_shader_use(&shader2d);
    
    ge_window_blending(&window, 1);
    ge_window_depth_test(&window, 0);
    ge_scene_render(&scene2d);
    ge_window_depth_test(&window, 1);
    ge_window_blending(&window, 0);
    
#if POSTPROCESSING
    ge_framebuffer_default();
    ge_shader_use(&fb_shader);
    
    ge_framebuffer_render(&framebuffer);
#endif
    
    x += 143*delta;
    while(x > 360) x -= 360;
}

void resize(void *data, int w, int h) {
    (void)data;
    ge_window_view(&window, w, h);
    ge_camera_perspective(&camera3d, 72, w/(float)h, 1000, 0.1);
    GE_CAMERA_ORTHO2D(&camera2d, 0, 0, w, h);
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
    
    if(ge_window_set_callbacks(&window, draw, resize, keypress, mouseevent)){
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

