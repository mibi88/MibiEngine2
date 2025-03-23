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

#include <stdlib.h>

#define GE_STDMODEL_ADD_ARRAY(model, array, pos, data, type, num, item_size) \
    if(ge_modelarray_init(&array, data, type, num, item_size)){ \
        return 1; \
    } \
    model->arrays[pos] = &array;

void _ge_gles_stdmodel_after_free(void *_model, void *extra) {
    (void)_model;
    free(extra);
}

int _ge_gles_stdmodel_init(GEModel *model, void *indices, void *vertices,
                           GEType index_type, GEType vertex_type,
                           size_t index_num, size_t vertex_num,
                           size_t item_size, void *extra) {
    size_t i;
    GEStdModel *stdmodel;
    
    stdmodel = malloc(sizeof(GEStdModel));
    if(stdmodel == NULL){
        return 1;
    }
    
    for(i=0;i<GE_STDMODEL_ARRAY_NUM;i++){
        stdmodel->arrays[i] = NULL;
        stdmodel->array_attrs[i] = NULL;
    }
    
    if(ge_modelarray_init(&stdmodel->vertex_array, vertices, vertex_type,
                          vertex_num, item_size)){
        free(stdmodel);
        return 2;
    }
    if(ge_model_init(model, stdmodel->arrays, GE_STDMODEL_ARRAY_NUM,
                     indices, index_type, index_num, stdmodel)){
        ge_modelarray_free(&stdmodel->vertex_array);
        free(stdmodel);
        return 3;
    }
    
    stdmodel->arrays[0] = &stdmodel->vertex_array;
    
    if(ge_model_set_attr(model, &stdmodel->attr)){
        ge_model_free(model);
        free(stdmodel);
        return 4;
    }
    if(ge_model_set_callbacks(model, NULL, NULL, NULL,
                              _ge_gles_stdmodel_after_free, 1)){
        ge_model_free(model);
        free(stdmodel);
        return 5;
    }
#if GE_STDMODEL_INHERIT_LEVEL+1 >= GE_MODEL_INHERIT_MAX
    Stop compiling right now!
#endif
    model->extra[GE_STDMODEL_INHERIT_LEVEL+1] = extra;
    
    stdmodel->array_attrs[0] = &stdmodel->vertex_pos;
    stdmodel->array_attrs[1] = NULL;
    stdmodel->array_attrs[2] = NULL;
    stdmodel->array_attrs[3] = NULL;
    return 0;
}

int _ge_gles_stdmodel_shader_attr(GEModel *model, GEShader *shader,
                                  char **attr_names) {
    GEStdModel *stdmodel = model->extra[GE_STDMODEL_INHERIT_LEVEL];
    if(ge_model_attr_init(&stdmodel->attr, shader, stdmodel->array_attrs,
                          attr_names, GE_STDMODEL_ARRAY_NUM)){
        return 1;
    }
    return 0;
}

int _ge_gles_stdmodel_add_color(GEModel *model, void *data, GEType type,
                                size_t num, size_t item_size) {
    GEStdModel *stdmodel = model->extra[GE_STDMODEL_INHERIT_LEVEL];
    GE_STDMODEL_ADD_ARRAY(stdmodel, stdmodel->color_array, 1, data, type, num,
                          item_size);
    stdmodel->array_attrs[1] = &stdmodel->color_pos;
    return 0;
}

int _ge_gles_stdmodel_add_uv_coords(GEModel *model, void *data, GEType type,
                                    size_t num, size_t item_size) {
    GEStdModel *stdmodel = model->extra[GE_STDMODEL_INHERIT_LEVEL];
    GE_STDMODEL_ADD_ARRAY(stdmodel, stdmodel->uv_array, 2, data, type, num,
                          item_size);
    stdmodel->array_attrs[2] = &stdmodel->uv_pos;
    return 0;
}

int _ge_gles_stdmodel_add_normals(GEModel *model, void *data, GEType type,
                                  size_t num, size_t item_size) {
    GEStdModel *stdmodel = model->extra[GE_STDMODEL_INHERIT_LEVEL];
    GE_STDMODEL_ADD_ARRAY(stdmodel, stdmodel->normal_array, 3, data, type, num,
                          item_size);
    stdmodel->array_attrs[3] = &stdmodel->normal_pos;
    return 0;
}

