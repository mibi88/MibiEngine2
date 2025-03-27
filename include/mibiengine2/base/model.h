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

#ifndef GE_MODEL_H
#define GE_MODEL_H

#include <mibiengine2/base/mat.h>
#include <mibiengine2/base/types.h>
#include <mibiengine2/base/shader.h>
#include <mibiengine2/base/modelarray.h>

#include <mibiengine2/base/base.h>

typedef struct {
    GEModelArrayAttr **array_pos;
    size_t array_num;
} GEModelAttr;

#define GE_MODEL_INHERIT_MAX 8
#define GE_MODEL_INHERIT_LEVEL -1

typedef struct {
    GEModelArray **arrays;
    size_t array_num;
    /* Index array */
    struct {
        void *data;
        size_t num;
        GEType type;
        unsigned int vbo;
    } indices;
    GEModelAttr *attr;
    BASE_DATA(MODEL, {
        void (*before_rendering)(void *_model, GEModelAttr *attr,
                                 void *extra);
        void (*after_rendering)(void *_model, GEModelAttr *attr, void *extra);
        void (*before_free)(void *_model, void *_extra);
        void (*after_free)(void *_model, void *_extra);
    });
} GEModel;

/* ge_model_init
 * 
 * Initialize a model model with the arrays arrays (see modelarray.h), that
 * correspond to VBOs in the OpenGL ES backend.
 * These arrays can then be used by the shaders to get data such as positions,
 * uv coordinates, colors and normals.
 * 
 * arrays:     The arrays bound to this model. They should outlive the model.
 * array_num:  The number of arrays.
 * indices:    The indices used to get the data in the model arrays.
 * index_type: The type of the indices (see type.h).
 * index_num:  The number of indices.
 * extra:      Pointer stored in the model struct. Allows creation of higher
 *             level functions to create models (see base.h).
 * Returns 0 on success and a non zero int on failure.
 */
int ge_model_init(GEModel *model, GEModelArray **arrays, size_t array_num,
                  void *indices, GEType index_type, size_t index_num,
                  void *extra);

/* ge_model_set_attr
 *
 * Set the model attributes used when rendering this model. The model
 * attributes contain the position of the arrays in the shader for each model
 * array. Model attributes are required for rendering.
 *
 * model: The model to set the attributes to.
 * attr:  The model attributes.
 * Returns 0 on success and a non zero int on failure.
 */
int ge_model_set_attr(GEModel *model, GEModelAttr *attr);

/* ge_model_set_callbacks
 *
 * Set callbacks used to extend the model. They are used to bind the texture
 * and unbind it, for example, in texturedmodel.c.
 * It is possible to set multiple callbacks for a single event
 * (GE_MODEL_INHERIT_MAX at most). inc_ptr adds the callbacks if true, else the
 * previously set callbacks are replaced.
 * All the callbacks get called at some point if they are not set to NULL.
 * The model gets passed to them (as a void* pointer), followd by the model
 * attributes (see ge_model_set_attr), and another pointer for the extra data
 * (see ge_model_init).
 *
 * model:            The model to set the callbacks to.
 * before_rendering: The callback called before rendering.
 * after_rendering:  The callback called after rendering.
 * before_free:      The callback called before freeing the model (see
 *                   ge_model_free).
 * after_free:       The callback called after freeing the model (see
 *                   ge_model_free).
 * inc_ptr:          Zero if the previously set callbacks should be replaced,
 *                   else the callbacks will be added.
 * Returns 0 on success and a non zero int on failure.
 */
int ge_model_set_callbacks(GEModel *model,
                           void (*before_rendering)(void*, GEModelAttr*,
                                                    void*),
                           void (*after_rendering)(void*, GEModelAttr*,
                                                    void*),
                           void (*before_free)(void*, void*),
                           void (*after_free)(void*, void*),
                           int inc_ptr);

/* ge_model_get_callptr
 *
 * Get the number of currently set callbacks.
 *
 * model: The model to get the number of callbacks set from.
 * Returns the number of callbacks set to the model.
 */
size_t ge_model_get_callptr(GEModel *model);

/* ge_model_render
 *
 * Render a model. All the transformation matrices and other uniforms should be
 * loaded before calling this functions.
 *
 * model: The model to render.
 */
void ge_model_render(GEModel *model);

/* ge_model_render_multiple
 *
 * Render a model multiple times.
 * before and after may not be called in the correct order.
 *
 * model:         The model to render.
 * pos:           The position of the uniform variables that should be sent for
 *                each instance.
 * types:         The type of the uniforms.
 * uniforms:      Pointers to the unforms for each model.
 * uniform_count: The number of uniforms per model.
 * count:         The number of instances of the model to render.
 */
void ge_model_render_multiple(GEModel *model, GEShaderPos **pos,
                              GEUniformType *types, void ***uniforms,
                              size_t uniform_count, size_t count);

/* ge_model_attr_init
 *
 * Initialize model attributes (see ge_model_set_attr).
 *
 * attr:       The model attributes to initialize.
 * shader:     The shader that will be used with them, to render the model.
 * array_attr: The attributes for each model array. it should outlive the
 *             model attributes.
 * names:      The names of the attributes in the shader.
 * num:        The number of model array attributes.
 * Returns 0 on success and a non zero int on failure.
 */
int ge_model_attr_init(GEModelAttr *attr, GEShader *shader,
                       GEModelArrayAttr **array_attr, char **names,
                       size_t num);

/* ge_model_free
 *
 * Free the model with the model arrays associed to it.
 */
void ge_model_free(GEModel *model);

#endif
