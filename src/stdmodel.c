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

#include <stdmodel.h>

#include <gles.h>

#include <stdlib.h>

int ge_stdmodel_init(GEModel *model, void *indices, void *vertices,
                     GEType index_type, GEType vertex_type, size_t index_num,
                     size_t vertex_num, size_t item_size, void *extra) {
    return _ge_gles_stdmodel_init(model, indices, vertices, index_type,
                                  vertex_type, index_num, vertex_num,
                                  item_size, extra);
}

int ge_stdmodel_shader_attr(GEModel *model, GEShader *shader,
                            char **attr_names) {
    return _ge_gles_stdmodel_shader_attr(model, shader, attr_names);
}

int ge_stdmodel_add_color(GEModel *model, void *data, GEType type, size_t num,
                          size_t item_size) {
    return _ge_gles_stdmodel_add_color(model, data, type, num, item_size);
}

int ge_stdmodel_add_uv_coords(GEModel *model, void *data, GEType type,
                              size_t num, size_t item_size) {
    return _ge_gles_stdmodel_add_uv_coords(model, data, type, num, item_size);
}

int ge_stdmodel_add_normals(GEModel *model, void *data, GEType type,
                            size_t num, size_t item_size) {
    return _ge_gles_stdmodel_add_normals(model, data, type, num, item_size);
}

