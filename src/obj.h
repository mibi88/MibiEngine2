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

#ifndef GE_OBJ_H
#define GE_OBJ_H

#define GE_OBJ_DEBUG 0

#define GE_OBJ_ALLOC_STEP (1<<16)
#define GE_OBJ_TOK_SZ 32
#define GE_OBJ_TOK_MAX 8

#include <types.h>

typedef struct {
    float *vertices;
    float *uv_coords;
    float *normals;
    unsigned int *indices;
    
    size_t vertex_num;
    size_t uv_num;
    size_t normal_num;
    size_t index_num;
    
    size_t vertex_max_num;
    size_t uv_max_num;
    size_t normal_max_num;
    size_t index_max_num;
} GEObj;

/* ge_obj_init
 *
 * Load model data from an .obj file.
 *
 * obj:  The struct that will contain the model data.
 * data: The content of the .obj file.
 * size: The size of the data.
 * Returns 0 on success or an error code on failure.
 */
int ge_obj_init(GEObj *obj, char *data, size_t size);

/* ge_obj_free
 *
 * Free the obj model data.
 *
 * obj: The obj model data to free.
 */
void ge_obj_free(GEObj *obj);

#endif

