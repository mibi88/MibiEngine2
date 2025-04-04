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

#ifndef GE_BACKEND_H
#define GE_BACKEND_H

#include <mibiengine2/base/model.h>
#include <mibiengine2/base/mat.h>
#include <mibiengine2/base/window.h>
#include <mibiengine2/base/types.h>
#include <mibiengine2/base/shader.h>
#include <mibiengine2/base/texture.h>
#include <mibiengine2/base/stdmodel.h>
#include <mibiengine2/base/texturedmodel.h>
#include <mibiengine2/base/framebuffer.h>
#include <mibiengine2/base/shadertree.h>

typedef struct {
    int (*framebuffer_init)(GEFramebuffer *framebuffer, int w, int h,
                            size_t tex_count, GEColor *formats,
                            GETexType *type, char *linear);
    int (*framebuffer_resize)(GEFramebuffer *framebuffer, int w, int h);
    int (*framebuffer_attr)(GEFramebuffer *framebuffer, GEShader *shader,
                            char **attr_names, char **tex_names,
                            GEShaderPos *size_pos);
    void (*framebuffer_render)(GEFramebuffer *framebuffer);
    void (*framebuffer_use)(GEFramebuffer *framebuffer);
    void (*framebuffer_default)(void);
    void (*framebuffer_free)(GEFramebuffer *framebuffer);

    int (*model_init)(GEModel *model, GEModelArray **arrays, size_t array_num,
                      void *indices, GEType index_type, size_t index_num,
                      int updatable, void *extra);
    int (*model_update_indices)(GEModel *model, void *data, size_t size);
    int (*model_set_attr)(GEModel *model, GEModelAttr *attr);
    void (*model_render)(GEModel *model);
    void (*model_render_multiple)(GEModel *model, GEShaderPos **pos,
                                  GEUniformType *types, void **uniforms,
                                  size_t uniform_count, size_t count);
    int (*model_attr_init)(GEModelAttr *attr, GEShader *shader,
                           GEModelArrayAttr **array_attr, char **names,
                           size_t num);
    void (*model_free)(GEModel *model);

    int (*modelarray_init)(GEModelArray *array, void *data, GEType type,
                           size_t size, size_t item_size, int updatable);
    int (*modelarray_update)(GEModelArray *array, void *data, size_t size);
    int (*modelarray_enable)(GEModelArray *array, GEModelArrayAttr *attr);
    int (*modelarray_disable)(GEModelArray *array);
    void (*modelarray_free)(GEModelArray *array);

    char *(*shader_init)(GEShader *shader, char *vertex_source,
                         char *fragment_source);
    void (*shader_use)(GEShader *shader);
    GEShaderPos (*shader_get_pos)(GEShader *shader, char *name);
    void (*shader_load_mat4)(GEShaderPos *pos, GEMat4 *mat);
    void (*shader_load_mat3)(GEShaderPos *pos, GEMat3 *mat);
    void (*shader_load_vec4)(GEShaderPos *pos, GEVec4 *vec);
    void (*shader_load_vec3)(GEShaderPos *pos, GEVec3 *vec);
    void (*shader_load_vec2)(GEShaderPos *pos, GEVec2 *vec);
    void (*shader_free)(GEShader *shader);

    int (*texture_init)(GETexture *texture, GEImage *image, int linear,
                        int flip);
    int (*texture_update)(GETexture *texture, GEImage *image);
    void (*texture_use)(GETexture *texture, GEShaderPos *pos, size_t n);
    void (*texture_free)(GETexture *texture);

    int (*window_init)(GEWindow *window, char *title);
    int (*window_set_data)(GEWindow *window, void *data);
    int (*window_cap_framerate)(GEWindow *window, int cap);
    void (*window_depth_test)(GEWindow *window, int depth_test);
    void (*window_blending)(GEWindow *window, int blend);
    unsigned long (*window_ms)(GEWindow *window);
    int (*window_key_pressed)(GEWindow *window, GEKey key);
    void (*window_mainloop)(GEWindow *window);
    void (*window_clear)(GEWindow *window, float r, float g, float b, float a);
    void (*window_view)(GEWindow *window, int w, int h);
    void (*window_free)(GEWindow *window);
} GEBackend;

#endif

