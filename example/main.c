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

#define PRINT_MS 1

#define ARRAY_NUM 4

#define POSTPROCESSING 1

#define EXIT(rc) {free_on_exit(); exit(rc);}

/*
 * Useful links:
 * https://www.saschawillems.de/blog/2015/04/19/using-opengl-es-on-windows-desk
 * tops-via-egl/
 * https://jonmacey.blogspot.com/2012/05/getting-started-with-egl-on-raspberry.
 * html
 * https://www.learnopengles.com/android-lesson-one-getting-started/
 */

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

void free_on_exit(void) {
    puts("Free everything!");
    ge_model_free(&model);
    ge_obj_free(&obj);
    ge_texture_free(&texture);
    ge_image_free(&image);
    ge_shader_free(&shader);
    ge_shader_free(&fb_shader);
    ge_framebuffer_free(&framebuffer);
    ge_window_free(&window);
    puts("Successfully freed everything!");
}

char *load_text(char *file, size_t *size_ptr) {
    FILE *fp;
    char *data;
    size_t size;
    
    fp = fopen(file, "r");
    if(fp == NULL){
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);
    data = malloc(size+1);
    if(data == NULL){
        fclose(fp);
        return NULL;
    }
    fread(data, 1, size, fp);
    
    fclose(fp);
    data[size] = '\0';
    if(size_ptr) *size_ptr = size;
    return data;
}

void load_shader(GEShader *shader, char *vertex_file, char *fragment_file) {
    char *vertex_shader;
    char *fragment_shader;
    char *log;
    vertex_shader = load_text(vertex_file, NULL);
    fragment_shader = load_text(fragment_file, NULL);
    if(vertex_shader == NULL || fragment_shader == NULL){
        fputs("Shaders not found!\n", stderr);
        free(vertex_shader);
        free(fragment_shader);
        EXIT(EXIT_FAILURE);
    }
    
    if((log = ge_shader_init(shader, vertex_shader, fragment_shader))){
        fputs("Failed to load shaders:\n", stderr);
        fputs(log, stderr);
        free(vertex_shader);
        free(fragment_shader);
        EXIT(EXIT_FAILURE);
    }
    free(vertex_shader);
    free(fragment_shader);
}

void init(void) {
    GEColor colors[2] = {GE_C_RGBA, GE_C_RGBA};
    GETexType tex_types[2] = {GE_TEX_COLOR, GE_TEX_DEPTH};
    char linear[2] = {1, 0};
    char *attr_names[] = {
        "vertex",
        NULL,
        "uv",
        NULL
    };
    char *tex_names[] = {
        "color",
        "depth"
    };
    last_time = get_ms();
    
    load_shader(&shader, "shaders/vertex_3d.vert", "shaders/fragment_3d.frag");
    load_shader(&fb_shader, "shaders/vertex_fb.vert",
                "shaders/fragment_fb.frag");
    projection_mat_pos = ge_shader_get_pos(&shader, "projection_mat");
    view_mat_pos = ge_shader_get_pos(&shader, "view_mat");
    model_mat_pos = ge_shader_get_pos(&shader, "model_mat");
    normal_mat_pos = ge_shader_get_pos(&shader, "normal_mat");
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
    if(ge_framebuffer_attr(&framebuffer, &fb_shader, attr_names, tex_names,
                           &fb_size_pos)){
        fputs("Failed to initialize framebuffer attr!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    
    ge_shader_use(&shader);
}

void load_texture(void) {
    if(ge_image_init(&image, "spot_texture.png")){
        fputs("Failed to read image!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
    if(ge_texture_init(&texture, &image, 1, 0)){
        fputs("Failed to load texture!\n", stderr);
        EXIT(EXIT_FAILURE);
    }
}

void load_model(void) {
    void *data;
    float *colors;
    size_t size;
    size_t i;
    char *attr_names[ARRAY_NUM] = {
        "vertex",
        "color",
        "uv",
        "normal"
    };
    
    data = load_text("spot.obj", &size);
    if(data == NULL) EXIT(EXIT_FAILURE);
    
    ge_obj_init(&obj, data, size);
    colors = malloc(obj.vertex_num*sizeof(float));
    for(i=0;i<obj.vertex_num;i++){
        if((i&3) == 3) colors[i] = 1;
        else colors[i] = 0.5;
    }
    
    if(ge_texturedmodel_init(&model, &texture, obj.indices, obj.vertices,
                             GE_T_UINT, GE_T_FLOAT, obj.index_num,
                             obj.vertex_num, 4, NULL)){
        fputs("Failed to init model!\n", stderr);
        free(colors);
        free(data);
        EXIT(EXIT_FAILURE);
    }
    if(ge_stdmodel_add_color(&model, colors, GE_T_FLOAT, obj.vertex_num, 4)){
        fputs("Failed to add colors!\n", stderr);
    }
    if(ge_stdmodel_add_uv_coords(&model, obj.uv_coords, GE_T_FLOAT, obj.uv_num,
                                3)){
        fputs("Failed to add UV coords!\n", stderr);
    }
    if(ge_stdmodel_add_normals(&model, obj.normals, GE_T_FLOAT, obj.normal_num,
                              3)){
        fputs("Failed to add normals!\n", stderr);
    }
    if(ge_stdmodel_shader_attr(&model, &shader, attr_names)){
        fputs("Failed to get array shader pos!\n", stderr);
    }
    if(ge_texturedmodel_set_texture(&model, &model_tex_pos)){
        fputs("Failed to set model texture!\n", stderr);
    }
    free(colors);
    free(data);
}

float x = 0;

void draw(void *data) {
    GEMat4 tmp1, tmp2;
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
    
    ge_mat4_translate3d(&tmp1, 0, 0, -2.2);
    ge_mat4_scale3d(&tmp2, 1, 1, 1);
    ge_mat4_mmul(&model_mat, &tmp1, &tmp2);
    tmp1 = model_mat;
    ge_mat4_rot3d(&tmp2, GE_A_Y, x);
    ge_mat4_mmul(&model_mat, &tmp1, &tmp2);
    tmp1 = model_mat;
    ge_mat4_rot3d(&tmp2, GE_A_X, 0);
    ge_mat4_mmul(&model_mat, &tmp1, &tmp2);
    tmp1 = model_mat;
    ge_mat4_rot3d(&tmp2, GE_A_Z, 0);
    ge_mat4_mmul(&model_mat, &tmp1, &tmp2);
    tmp1 = model_mat;
    ge_mat4_translate3d(&tmp2, 0, 0, 0);
    ge_mat4_mmul(&model_mat, &tmp1, &tmp2);
    
    /* ge_mat4_inverse(&tmp1, &model_mat);
    ge_mat4_transpose(&normal_mat, &tmp1); */
    /* TODO: Create the normal matrix */
    ge_mat3_mat4(&normal_mat, &model_mat);
    
    ge_shader_load_mat4(&projection_mat_pos, &projection_mat);
    ge_shader_load_mat4(&view_mat_pos, &view_mat);
    ge_shader_load_mat4(&model_mat_pos, &model_mat);
    ge_shader_load_mat3(&normal_mat_pos, &normal_mat);
    ge_shader_load_vec2(&uv_max_pos, &texture.uv_max);
    
    ge_model_render(&model);
    
#if POSTPROCESSING
    ge_framebuffer_default();
    ge_shader_use(&fb_shader);
    
    ge_framebuffer_render(&framebuffer);
#endif
    
    x += 2.5*delta;
    if(x > 360) x -= 360;
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
    load_texture();
    load_model();
    
    window.draw = draw;
    window.resize = resize;
    window.data = &window;
    ge_window_mainloop(&window);
    
    free_on_exit();
    
    return EXIT_SUCCESS;
}

