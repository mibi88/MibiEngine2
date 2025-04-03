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

#ifndef GE_MODELARRAY_H
#define GE_MODELARRAY_H

#define GE_MODELARRAY_AVAILABLE 1

#include <mibiengine2/base/types.h>

typedef struct {
    int pos;
} GEModelArrayAttr;

typedef struct {
    void *data;
    GEType type;
    unsigned int vbo;
    size_t size;
    size_t item_size;
    GEModelArrayAttr *current_attr;
    unsigned char updatable;
} GEModelArray;

/* ge_modelarray_init
 *
 * Intialize a model array. They correspond to VBOs in the OpenGL ES backend.
 * Model arrays are used in models to provide data to shaders (see model.h).
 * They may contain the vertices, colors, uv coordinates or normals used in the
 * shaders.
 *
 * array:     The array to initalize.
 * data:      The data to load into the array. It should outlive the array.
 * type:      The type of the data loaded into the array (see type.h).
 * size:      The size of the data.
 * item_size: The number of elements read at once: 2 for a vec2, 3 for a vec3,
 *            4 for a vec4, etc.
 * updatable: Should be zero if it should not be modifiable or non-zero if it
 *            can be updated.
 * Returns GE_E_NONE (0) on success or an error code on failure.
 */
int ge_modelarray_init(GEModelArray *array, void *data, GEType type,
                       size_t size, size_t item_size, int updatable);

/* ge_modelarray_update
 *
 * Update a modelarray's data.
 *
 * array: The modelarray.
 * data:  The data to load.
 * size:  The number of elements in the data (the type of the data is kept).
 * Returns GE_E_NONE (0) on success or an error code on failure.
 */
int ge_modelarray_update(GEModelArray *array, void *data, size_t size);

/* ge_modelarray_enable
 *
 * Use this model array with the model attributes in attr.
 * Model attributes share the position where the model array needs to be loaded
 * to for use in a shader.
 *
 * array: The array to use.
 * attr:  The array attributes.
 * Returns GE_E_NONE (0) on success or an error code on failure.
 */
int ge_modelarray_enable(GEModelArray *array, GEModelArrayAttr *attr);

/* ge_modelarray_disable
 *
 * Stop using this array.
 *
 * array: The array to stop using.
 * Returns GE_E_NONE (0) on success or an error code on failure.
 */
int ge_modelarray_disable(GEModelArray *array);

/* ge_modelarray_free
 *
 * Free a model array. If it is used by a model it will already be freed by the
 * model that is using it.
 *
 * array: The array to free.
 */
void ge_modelarray_free(GEModelArray *array);

#endif

