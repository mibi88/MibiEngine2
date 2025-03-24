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

#include <mibiengine2/base/model.h>

#include <gles.h>

#define DEF_CASE(d) case d: return #d;

int ge_model_init(GEModel *model, GEModelArray **arrays, size_t array_num,
                  void *indices, GEType index_type, size_t index_num,
                  void *extra) {
    return _ge_gles_model_init(model, arrays, array_num, indices, index_type,
                               index_num, extra);
}

int ge_model_set_attr(GEModel *model, GEModelAttr *attr) {
    return _ge_gles_model_set_attr(model, attr);
}

int ge_model_set_callbacks(GEModel *model,
                           void (*before_rendering)(void*, GEModelAttr*,
                                                    void*),
                           void (*after_rendering)(void*, GEModelAttr*,
                                                    void*),
                           void (*before_free)(void*, void*),
                           void (*after_free)(void*, void*),
                           int inc_ptr) {
    model->calls[model->call_ptr].before_rendering = before_rendering;
    model->calls[model->call_ptr].after_rendering = after_rendering;
    model->calls[model->call_ptr].before_free = before_free;
    model->calls[model->call_ptr].after_free = after_free;
    if(inc_ptr){
        model->call_ptr++;
        if(model->call_ptr >= GE_MODEL_INHERIT_MAX){
            model->call_ptr = GE_MODEL_INHERIT_MAX-1;
        }
    }
    return 0;
}

size_t ge_model_get_callptr(GEModel *model) {
    return model->call_ptr;
}

void ge_model_render(GEModel *model) {
    _ge_gles_model_render(model);
}

int ge_model_attr_init(GEModelAttr *attr, GEShader *shader,
                       GEModelArrayAttr **array_attr, char **names,
                       size_t num) {
    return _ge_gles_model_attr_init(attr, shader, array_attr, names, num);
}

void ge_model_free(GEModel *model) {
    _ge_gles_model_free(model);
}

