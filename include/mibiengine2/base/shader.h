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

#ifndef GE_SHADER_H
#define GE_SHADER_H

#define GE_SHADER_LOG_SIZE 512

#include <mibiengine2/base/mat.h>

typedef struct {
    int shader_program;
    int vertex_shader, fragment_shader;
} GEShader;

typedef struct {
    int pos;
} GEShaderPos;

typedef enum {
    GE_U_MAT4,
    GE_U_MAT3,
    GE_U_VEC4,
    GE_U_VEC3,
    GE_U_VEC2,
    GE_U_AMOUNT
} GEUniformType;

/* ge_shader_init
 *
 * Create a shader program made up of a vertex shader and a fragment shader.
 *
 * shader:          The shader program data.
 * vertex_source:   The vertex shader source code.
 * fragment_source: The fragment shader source code.
 * Returns NULL on success or a pointer to the error log (a null terminated
 * string). Its size is SHADER_LOG_SIZE.
 */
char *ge_shader_init(GEShader *shader, char *vertex_source,
                     char *fragment_source);

/* ge_shader_use
 *
 * Use a shader.
 *
 * shader: The shader to use.
 */
void ge_shader_use(GEShader *shader);

/* ge_shader_get_pos
 *
 * Get the position of a uniform variable in a shader.
 *
 * shader: The shader to get the position of the uniform variable from.
 * name:   The name of the uniform variable.
 * Returns the position of the uniform variable.
 */
GEShaderPos ge_shader_get_pos(GEShader *shader, char *name);

/* ge_shader_load_mat4
 *
 * Load a 4x4 matrix (see mat.h) to the currently used shader.
 *
 * pos: The position of the uniform variable.
 * mat: The matrix to load.
 */
void ge_shader_load_mat4(GEShaderPos *pos, GEMat4 *mat);

/* ge_shader_load_mat3
 *
 * Load a 3x3 matrix (see mat.h) to the currently used shader.
 *
 * pos: The position of the uniform variable.
 * mat: The matrix to load.
 */
void ge_shader_load_mat3(GEShaderPos *pos, GEMat3 *mat);

/* ge_shader_load_vec4
 *
 * Load a 4D vector (see mat.h) to the currently used shader.
 *
 * pos: The position of the uniform variable.
 * vec: The vector to load.
 */
void ge_shader_load_vec4(GEShaderPos *pos, GEVec4 *vec);

/* ge_shader_load_vec3
 *
 * Load a 3D vector (see mat.h) to the currently used shader.
 *
 * pos: The position of the uniform variable.
 * vec: The vector to load.
 */
void ge_shader_load_vec3(GEShaderPos *pos, GEVec3 *vec);

/* ge_shader_load_vec2
 *
 * Load a 2D vector (see mat.h) to the currently used shader.
 *
 * pos: The position of the uniform variable.
 * vec: The vector to load.
 */
void ge_shader_load_vec2(GEShaderPos *pos, GEVec2 *vec);

/* ge_shader_load_any
 *
 * Load a uniform variable to the currently used shader.
 *
 * pos:  The position of the uniform variable.
 * type: The type of the uniform variable.
 * var:  The uniform to load.
 */
void ge_shader_load_any(GEShaderPos *pos, GEUniformType type, void *var);

/* ge_shader_free
 *
 * Delete a shader.
 *
 * shader: The shader to delete.
 */
void ge_shader_free(GEShader *shader);

#endif
