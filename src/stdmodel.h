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

#ifndef GE_STDMODEL_H
#define GE_STDMODEL_H

#define GE_STDMODEL_ARRAY_NUM 4
#define GE_STDMODEL_INHERIT_LEVEL (GE_MODEL_INHERIT_LEVEL+1)

#include <model.h>

/* This struct will be passed as the extra arg when calling ge_model_init to
 * store data about this type of model. */
typedef struct {
    /* Arrays */
    GEModelArray vertex_array;
    GEModelArray color_array;
    GEModelArray uv_array;
    GEModelArray normal_array;
    GEModelArray *arrays[GE_STDMODEL_ARRAY_NUM];
    /* Array attributes */
    GEModelArrayAttr vertex_pos;
    GEModelArrayAttr color_pos;
    GEModelArrayAttr uv_pos;
    GEModelArrayAttr normal_pos;
    GEModelArrayAttr *array_attrs[GE_STDMODEL_ARRAY_NUM];
    GEModelAttr attr;
} GEStdModel;

/* ge_stdmodel_init
 *
 * Create a standard model. In comparison to GEModel, stdmodels are made to
 * store common 3D models, with vertices, uv coordinates, colors and/or
 * normals.
 * Creating raw GEModels should be only needed in very, very specific usecases.
 *
 * model:       The model to initialize.
 * indices:     The model indices (see model.h).
 * vertices:    The position of the vertices of the model.
 * index_type:  The type of the index data (see type.h).
 * vertex_type: The type of the vertex data (see type.h).
 * index_num:   The number of indices.
 * vertex_num:  The number of vertex positions.
 * item_size:   The size of a single vertex position.
 * extra:       Extra data (see model.h and base.h).
 * Returns 0 on success or an error code on failure.
 */
int ge_stdmodel_init(GEModel *model, void *indices, void *vertices,
                     GEType index_type, GEType vertex_type, size_t index_num,
                     size_t vertex_num, size_t item_size, void *extra);

/* ge_stdmodel_shader_attr
 *
 * Generate the model attributes.
 *
 * model:      The model to generate the model attributes of.
 * shader:     The shader that will be used to render this model.
 * attr_names: The names of the attributes. They should be in the following
 *             order: the vertex position attribute name, followed by the color
 *             attribute name, the UV coordinate attribute name and the normal
 *             attribute name.
 * Returns 0 on success or an error code on failure.
 */
int ge_stdmodel_shader_attr(GEModel *model, GEShader *shader,
                            char **attr_names);

/* ge_stdmodel_add_color
 *
 * Add color data to the model.
 *
 * model:     The model to add the color data to.
 * data:      The color data.
 * num:       The size of the data.
 * item_size: The size of a single color.
 * Returns 0 on success or an error code on failure.
 */
int ge_stdmodel_add_color(GEModel *model, void *data, GEType type, size_t num,
                          size_t item_size);

/* ge_stdmodel_add_uv_coords
 *
 * Add UV coordinate data to the model.
 *
 * model:     The model to add the uv coordinate data to.
 * data:      The UV coordinate data.
 * num:       The size of the data.
 * item_size: The size of a single uv coordinate.
 * Returns 0 on success or an error code on failure.
 */
int ge_stdmodel_add_uv_coords(GEModel *model, void *data, GEType type,
                              size_t num, size_t item_size);

/* ge_stdmodel_add_normals
 *
 * Add normal data to the model.
 *
 * model:     The model to add the normal data to.
 * data:      The normal data.
 * num:       The size of the data.
 * item_size: The size of a single normal.
 * Returns 0 on success or an error code on failure.
 */
int ge_stdmodel_add_normals(GEModel *model, void *data, GEType type,
                            size_t num, size_t item_size);

#endif

