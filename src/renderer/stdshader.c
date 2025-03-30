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

#include <mibiengine2/renderer/stdshader.h>
#include <mibiengine2/errors.h>

int ge_stdshader_init(GEStdShader *stdshader, GEShader *shader) {
    stdshader->shader = shader;
    stdshader->projection_mat = ge_shader_get_pos(shader,
                                                  GE_STDSHADER_PROJECTION_MAT);
    stdshader->view_mat = ge_shader_get_pos(shader, GE_STDSHADER_VIEW_MAT);
    stdshader->model_mat = ge_shader_get_pos(shader, GE_STDSHADER_MODEL_MAT);
    stdshader->normal_mat = ge_shader_get_pos(shader, GE_STDSHADER_NORMAL_MAT);
    
    stdshader->uv_max = ge_shader_get_pos(shader, GE_STDSHADER_UV_MAX);
    
    stdshader->light_pos = ge_shader_get_pos(shader, GE_STDSHADER_LIGHT_POS);
    stdshader->light_color = ge_shader_get_pos(shader,
                                               GE_STDSHADER_LIGHT_COLOR);
    stdshader->light_num = ge_shader_get_pos(shader, GE_STDSHADER_LIGHT_NUM);
    
    stdshader->texture = ge_shader_get_pos(shader, GE_STDSHADER_TEXTURE);
    return GE_E_SUCCESS;
}

GEShader *ge_stdshader_get_shader(GEStdShader *stdshader) {
    return stdshader->shader;
}

void ge_stdshader_free(GEStdShader *stdshader) {
    (void)stdshader;
    /* Nothing to free */
}

