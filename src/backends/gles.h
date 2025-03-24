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

#ifndef GE_GLES_H
#define GE_GLES_H

#include <mibiengine2/base/model.h>
#include <mibiengine2/base/mat.h>
#include <mibiengine2/base/window.h>
#include <mibiengine2/base/types.h>
#include <mibiengine2/base/shader.h>
#include <mibiengine2/base/obj.h>
#include <mibiengine2/base/texture.h>
#include <mibiengine2/base/image.h>
#include <mibiengine2/base/stdmodel.h>
#include <mibiengine2/base/texturedmodel.h>
#include <mibiengine2/base/framebuffer.h>

int _ge_gles_framebuffer_init(GEFramebuffer *framebuffer, int w, int h,
                              size_t tex_count, GEColor *formats,
                              GETexType *type, char *linear);
int _ge_gles_framebuffer_resize(GEFramebuffer *framebuffer, int w, int h);
int _ge_gles_framebuffer_attr(GEFramebuffer *framebuffer, GEShader *shader,
                              char **attr_names, char **tex_names,
                              GEShaderPos *size_pos);
void _ge_gles_framebuffer_render(GEFramebuffer *framebuffer);
void _ge_gles_framebuffer_use(GEFramebuffer *framebuffer);
void _ge_gles_framebuffer_default(void);
void _ge_gles_framebuffer_free(GEFramebuffer *framebuffer);

int _ge_gles_model_init(GEModel *model, GEModelArray **arrays,
                        size_t array_num, void *indices, GEType index_type,
                        size_t index_num, void *extra);
int _ge_gles_model_set_attr(GEModel *model, GEModelAttr *attr);
void _ge_gles_model_render(GEModel *model);
int _ge_gles_model_attr_init(GEModelAttr *attr, GEShader *shader,
                             GEModelArrayAttr **array_attr, char **names,
                             size_t num);
void _ge_gles_model_free(GEModel *model);

int _ge_gles_modelarray_init(GEModelArray *array, void *data, GEType type,
                             size_t size, size_t item_size);
int _ge_gles_modelarray_enable(GEModelArray *array, GEModelArrayAttr *attr);
int _ge_gles_modelarray_disable(GEModelArray *array);
void _ge_gles_modelarray_free(GEModelArray *array);

char *_ge_gles_shader_init(GEShader *shader, char *vertex_source,
                           char *fragment_source);
void _ge_gles_shader_use(GEShader *shader);
GEShaderPos _ge_gles_shader_get_pos(GEShader *shader, char *name);
void _ge_gles_shader_load_mat4(GEShaderPos *pos, GEMat4 *mat);
void _ge_gles_shader_load_mat3(GEShaderPos *pos, GEMat3 *mat);
void _ge_gles_shader_load_vec4(GEShaderPos *pos, GEVec4 *vec);
void _ge_gles_shader_load_vec3(GEShaderPos *pos, GEVec3 *vec);
void _ge_gles_shader_load_vec2(GEShaderPos *pos, GEVec2 *vec);
void _ge_gles_shader_free(GEShader *shader);

int _ge_gles_texture_init(GETexture *texture, GEImage *image, int linear,
                          int flip);
void _ge_gles_texture_use(GETexture *texture, GEShaderPos *pos, size_t n);
void _ge_gles_texture_free(GETexture *texture);

int _ge_gles_window_init(GEWindow *window, char *title);
void _ge_gles_window_mainloop(GEWindow *window);
void _ge_gles_window_clear(GEWindow *window, float r, float g, float b,
                           float a);
void _ge_gles_window_view(GEWindow *window, int w, int h);
void _ge_gles_window_free(GEWindow *window);

#endif
