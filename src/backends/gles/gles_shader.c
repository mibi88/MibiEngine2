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

#include <mibiengine2/base/shader.h>

#include <GLES2/gl2.h>
#include <stddef.h>

char *_ge_gles_shader_init(GEShader *shader, char *vertex_source,
                           char *fragment_source) {
    static char log[SHADER_LOG_SIZE];
    int success;
    shader->shader_program = glCreateProgram();
    shader->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader->vertex_shader, 1,
                  (const char* const*)&vertex_source, NULL);
    glCompileShader(shader->vertex_shader);
    /* Return the log if required */
    glGetShaderiv(shader->vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shader->vertex_shader, SHADER_LOG_SIZE, NULL, log);
        glDeleteShader(shader->vertex_shader);
        glDeleteProgram(shader->shader_program);
        return log;
    }
    shader->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader->fragment_shader, 1,
                   (const char* const*)&fragment_source, NULL);
    glCompileShader(shader->fragment_shader);
    /* Return the log if required */
    glGetShaderiv(shader->fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shader->fragment_shader, SHADER_LOG_SIZE, NULL,
                           log);
        glDeleteShader(shader->vertex_shader);
        glDeleteShader(shader->fragment_shader);
        glDeleteProgram(shader->shader_program);
        return log;
    }
    /* Link the shader program */
    glAttachShader(shader->shader_program, shader->vertex_shader);
    glAttachShader(shader->shader_program, shader->fragment_shader);
    glLinkProgram(shader->shader_program);
    glGetProgramiv(shader->shader_program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shader->shader_program, SHADER_LOG_SIZE, NULL,
                            log);
        glDeleteShader(shader->vertex_shader);
        glDeleteShader(shader->fragment_shader);
        glDeleteProgram(shader->shader_program);
        return log;
    }
    return NULL;
}

void _ge_gles_shader_use(GEShader *shader) {
    glUseProgram(shader->shader_program);
}

GEShaderPos _ge_gles_shader_get_pos(GEShader *shader, char *name) {
    GEShaderPos pos;
    pos.pos = glGetUniformLocation(shader->shader_program, name);
    return pos;
}

void _ge_gles_shader_load_mat4(GEShaderPos *pos, GEMat4 *mat) {
    glUniformMatrix4fv(pos->pos, 1, 0, mat->mat);
}

void _ge_gles_shader_load_mat3(GEShaderPos *pos, GEMat3 *mat) {
    glUniformMatrix3fv(pos->pos, 1, 0, mat->mat);
}

void _ge_gles_shader_load_vec2(GEShaderPos *pos, GEVec2 *vec) {
    glUniform2f(pos->pos, vec->x, vec->y);
}

void _ge_gles_shader_free(GEShader *shader) {
    glDeleteShader(shader->vertex_shader);
    shader->vertex_shader = 0;
    glDeleteShader(shader->fragment_shader);
    shader->fragment_shader = 0;
    glDeleteProgram(shader->shader_program);
    shader->shader_program = 0;
}

