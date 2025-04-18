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

#include <mibiengine2/base/stdmodel.h>

#include <mibiengine2/errors.h>

#include <stdlib.h>

#define GE_STDMODEL_ADD_ARRAY(model, array, pos, data, type, num, item_size) \
    if(ge_modelarray_init(&array, data, type, num, item_size, \
                          model->updatable)){ \
        return 1; \
    } \
    model->arrays[pos] = &array;

void _ge_stdmodel_after_free(void *_model, void *extra) {
    GEModel *model = _model;
    free(extra);
    model->extra[GE_STDMODEL_INHERIT_LEVEL] = NULL;
}

int ge_stdmodel_init(GEModel *model, void *indices, void *vertices,
                     GEType index_type, GEType vertex_type, size_t index_num,
                     size_t vertex_num, size_t item_size, int updatable,
                     void *extra) {
    size_t i;
    GEStdModel *stdmodel;
    
    stdmodel = malloc(sizeof(GEStdModel));
    if(stdmodel == NULL){
        return GE_E_OUT_OF_MEM;
    }
    
    /* TODO: Don't store updatable separately from model. */
    stdmodel->updatable = updatable;
    
    for(i=0;i<GE_STDMODEL_ARRAY_NUM;i++){
        stdmodel->arrays[i] = NULL;
        stdmodel->array_attrs[i] = NULL;
    }
    
    if(ge_modelarray_init(&stdmodel->vertex_array, vertices, vertex_type,
                          vertex_num, item_size, updatable)){
        free(stdmodel);
        return GE_E_MODELARRAY_INIT;
    }
    if(ge_model_init(model, stdmodel->arrays, GE_STDMODEL_ARRAY_NUM,
                     indices, index_type, index_num, updatable, stdmodel)){
        ge_modelarray_free(&stdmodel->vertex_array);
        free(stdmodel);
        return GE_E_MODEL_INIT;
    }
    
    stdmodel->arrays[0] = &stdmodel->vertex_array;
    
    if(ge_model_set_attr(model, &stdmodel->attr)){
        ge_model_free(model);
        free(stdmodel);
        return GE_E_SET_ATTR;
    }
    if(ge_model_set_callbacks(model, NULL, NULL, NULL, _ge_stdmodel_after_free,
                              1)){
        ge_model_free(model);
        free(stdmodel);
        return GE_E_SET_CALLBACKS;
    }
#if GE_STDMODEL_INHERIT_LEVEL+1 >= GE_MODEL_INHERIT_MAX
    Stop compiling right now!
#endif
    model->extra[GE_STDMODEL_INHERIT_LEVEL+1] = extra;
    
    stdmodel->array_attrs[0] = &stdmodel->vertex_pos;
    stdmodel->array_attrs[1] = NULL;
    stdmodel->array_attrs[2] = NULL;
    stdmodel->array_attrs[3] = NULL;
    return GE_E_NONE;
}

int ge_stdmodel_shader_attr(GEModel *model, GEShader *shader,
                            char **attr_names) {
    GEStdModel *stdmodel = model->extra[GE_STDMODEL_INHERIT_LEVEL];
    if(ge_model_attr_init(&stdmodel->attr, shader, stdmodel->array_attrs,
                          attr_names, GE_STDMODEL_ARRAY_NUM)){
        return GE_E_SET_ATTR;
    }
    return GE_E_NONE;
}

int ge_stdmodel_add_color(GEModel *model, void *data, GEType type, size_t num,
                          size_t item_size) {
    GEStdModel *stdmodel = model->extra[GE_STDMODEL_INHERIT_LEVEL];
    if(stdmodel->array_attrs[1] == NULL){
        GE_STDMODEL_ADD_ARRAY(stdmodel, stdmodel->color_array, 1, data, type,
                              num, item_size);
        stdmodel->array_attrs[1] = &stdmodel->color_pos;
        return GE_E_NONE;
    }
    return GE_E_ALREADY_ADDED;
}

int ge_stdmodel_add_uv_coords(GEModel *model, void *data, GEType type,
                              size_t num, size_t item_size) {
    GEStdModel *stdmodel = model->extra[GE_STDMODEL_INHERIT_LEVEL];
    if(stdmodel->array_attrs[2] == NULL){
        GE_STDMODEL_ADD_ARRAY(stdmodel, stdmodel->uv_array, 2, data, type, num,
                              item_size);
        stdmodel->array_attrs[2] = &stdmodel->uv_pos;
        return GE_E_NONE;
    }
    return GE_E_ALREADY_ADDED;
}

int ge_stdmodel_add_normals(GEModel *model, void *data, GEType type,
                            size_t num, size_t item_size) {
    GEStdModel *stdmodel = model->extra[GE_STDMODEL_INHERIT_LEVEL];
    if(stdmodel->array_attrs[3] == NULL){
        GE_STDMODEL_ADD_ARRAY(stdmodel, stdmodel->normal_array, 3, data, type,
                              num, item_size);
        stdmodel->array_attrs[3] = &stdmodel->normal_pos;
        return GE_E_NONE;
    }
    return GE_E_ALREADY_ADDED;
}

int ge_stdmodel_update_vertices(GEModel *model, void *data, size_t size) {
    GEStdModel *stdmodel = model->extra[GE_STDMODEL_INHERIT_LEVEL];
    if(stdmodel->array_attrs[0] == NULL) return GE_E_NOT_ADDED_YET;
    return ge_modelarray_update(&stdmodel->vertex_array, data, size);
}

int ge_stdmodel_update_color(GEModel *model, void *data, size_t size) {
    GEStdModel *stdmodel = model->extra[GE_STDMODEL_INHERIT_LEVEL];
    if(stdmodel->array_attrs[1] == NULL) return GE_E_NOT_ADDED_YET;
    return ge_modelarray_update(&stdmodel->color_array, data, size);
}

int ge_stdmodel_update_uv_coords(GEModel *model, void *data, size_t size) {
    GEStdModel *stdmodel = model->extra[GE_STDMODEL_INHERIT_LEVEL];
    if(stdmodel->array_attrs[2] == NULL) return GE_E_NOT_ADDED_YET;
    return ge_modelarray_update(&stdmodel->uv_array, data, size);
}

int ge_stdmodel_update_normals(GEModel *model, void *data, size_t size) {
    GEStdModel *stdmodel = model->extra[GE_STDMODEL_INHERIT_LEVEL];
    if(stdmodel->array_attrs[3] == NULL) return GE_E_NOT_ADDED_YET;
    return ge_modelarray_update(&stdmodel->normal_array, data, size);
}

