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

#ifndef GE_LOADER_H
#define GE_LOADER_H

#include <mibiengine2/base/model.h>
#include <mibiengine2/base/stdmodel.h>
#include <mibiengine2/base/texturedmodel.h>
#include <mibiengine2/base/texture.h>
#include <mibiengine2/base/shader.h>
#include <mibiengine2/base/obj.h>
#include <mibiengine2/renderer/renderable.h>
#include <mibiengine2/renderer/stdshader.h>
#include <mibiengine2/renderer/light.h>

typedef struct {
    GEModel *model;
    GEStdShader *shader;
} GEModelRenderable;

char *ge_loader_load_text(char *file, size_t *size_ptr);

int ge_loader_load_obj(GEModel *model, GEShader *shader, GETexture *texture,
                       char *file, char **attr_names, GEShaderPos *tex_pos,
                       GEShaderPos *uv_max_pos);

int ge_loader_load_stdobj(GEModel *model, GEStdShader *shader,
                          GETexture *texture, char *file);

int ge_loader_model_renderable(GERenderable *renderable, GEModel *model,
                               GEStdShader *shader);

int ge_loader_load_obj_as_renderable(GERenderable *renderable,
                                     GEStdShader *shader, GETexture *texture,
                                     char *file);

int ge_loader_light_renderable(GERenderable *renderable, GELight *light,
                               GEStdShader *shader);

char *ge_loader_load_shader(GEShader *shader, char *vertex_file,
                            char *fragment_file);

#endif

