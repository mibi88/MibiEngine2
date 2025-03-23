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

#include <modelarray.h>

#include <GLES2/gl2.h>

int _ge_gles_modelarray_init(GEModelArray *array, void *data, GEType type,
                             size_t size, size_t item_size) {
    /* TODO: Support updating the buffer */
    array->data = data;
    array->type = type;
    array->size = size;
    array->item_size = item_size;
    array->current_attr = NULL;
    
    glGenBuffers(1, &array->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, array->vbo);
    glBufferData(GL_ARRAY_BUFFER, size*ge_type_size[type],
                 data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return 0;
}

int _ge_gles_modelarray_enable(GEModelArray *array, GEModelArrayAttr *attr) {
    int gl_types[GE_T_AMOUNT] = {
        0,
        GL_BYTE,
        GL_UNSIGNED_BYTE,
        GL_SHORT,
        GL_UNSIGNED_SHORT,
        GL_INT,
        GL_UNSIGNED_INT,
        GL_INT,
        GL_UNSIGNED_INT,
        GL_FLOAT,
        GL_FLOAT
    };
    glBindBuffer(GL_ARRAY_BUFFER, array->vbo);
    glEnableVertexAttribArray(attr->pos);
    glVertexAttribPointer(attr->pos, array->item_size, gl_types[array->type],
                          GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    array->current_attr = attr;
    return 0;
}

int _ge_gles_modelarray_disable(GEModelArray *array) {
    if(array->current_attr == NULL) return 1;
    glDisableVertexAttribArray(array->current_attr->pos);
    array->current_attr = NULL;
    return 0;
}

void _ge_gles_modelarray_free(GEModelArray *array) {
    glDeleteBuffers(1, &array->vbo);
    array->vbo = 0;
}

