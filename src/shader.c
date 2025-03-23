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

#include <shader.h>

#include <gles.h>
#include <stddef.h>

char *ge_shader_init(GEShader *shader, char *vertex_source,
                     char *fragment_source) {
    return _ge_gles_shader_init(shader, vertex_source, fragment_source);
}

void ge_shader_use(GEShader *shader) {
    _ge_gles_shader_use(shader);
}

GEShaderPos ge_shader_get_pos(GEShader *shader, char *name) {
    return _ge_gles_shader_get_pos(shader, name);
}

void ge_shader_load_mat4(GEShaderPos *pos, GEMat4 *mat) {
    _ge_gles_shader_load_mat4(pos, mat);
}

void ge_shader_load_mat3(GEShaderPos *pos, GEMat3 *mat) {
    _ge_gles_shader_load_mat3(pos, mat);
}

void ge_shader_load_vec2(GEShaderPos *pos, GEVec2 *vec) {
    _ge_gles_shader_load_vec2(pos, vec);
}

void ge_shader_free(GEShader *shader) {
    _ge_gles_shader_free(shader);
}

