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

#ifndef GE_STDSHADER_H
#define GE_STDSHADER_H

#include <mibiengine2/base/shader.h>

#define GE_STDSHADER_VERTEX "ge_vertex"
#define GE_STDSHADER_COLOR "ge_color"
#define GE_STDSHADER_UV "ge_uv"
#define GE_STDSHADER_NORMAL "ge_normal"

#define GE_STDSHADER_PROJECTION_MAT "ge_projection_mat"
#define GE_STDSHADER_VIEW_MAT "ge_view_mat"
#define GE_STDSHADER_MODEL_MAT "ge_model_mat"
#define GE_STDSHADER_NORMAL_MAT "ge_normal_mat"

#define GE_STDSHADER_LIGHT_POS "ge_light_pos"
#define GE_STDSHADER_LIGHT_COLOR "ge_light_color"
#define GE_STDSHADER_LIGHT_NUM "ge_light_num"

typedef struct {
    GEShader *shader;
    GEShaderPos projection_mat;
    GEShaderPos view_mat;
    GEShaderPos model_mat;
    GEShaderPos normal_mat;
    
    GEShaderPos light_pos;
    GEShaderPos light_color;
    GEShaderPos light_num;
} GEStdShader;

int ge_stdshader_init(GEStdShader *stdshader, GEShader *shader);

GEShader *ge_stdshader_get_shader(GEStdShader *stdshader);

void ge_stdshader_free(GEStdShader *stdshader);

#endif

