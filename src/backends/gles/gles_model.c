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

#include <GLES2/gl2.h>

#include <mibiengine2/errors.h>

#define DEF_CASE(d) case d: return #d;

char *_ge_gl_error_str(int error) {
    switch(error){
        DEF_CASE(GL_NO_ERROR)
        DEF_CASE(GL_INVALID_ENUM)
        DEF_CASE(GL_INVALID_VALUE)
        DEF_CASE(GL_INVALID_OPERATION)
        DEF_CASE(GL_INVALID_FRAMEBUFFER_OPERATION)
        DEF_CASE(GL_OUT_OF_MEMORY)
    }
    return NULL;
}

int _ge_gles_model_init(GEModel *model, GEModelArray **arrays,
                        size_t array_num, void *indices, GEType index_type,
                        size_t index_num, int updatable, void *extra) {
    size_t i;
    
    /* TODO: Support updating the index array */
    model->indices.data = indices;
    model->indices.type = index_type;
    
    model->indices.num = index_num;
    
    model->arrays = arrays;
    model->array_num = array_num;
    
    model->updatable = updatable;
    
    /* Index array */
    if(model->indices.data){
        glGenBuffers(1, &model->indices.vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->indices.vbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     index_num*ge_type_size[index_type], indices,
                     model->updatable ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    model->extra[0] = extra;
    /* The model attributes are required for rendering but need to be set
     * later */
    model->attr = NULL;
    for(i=0;i<GE_MODEL_INHERIT_MAX;i++){
        model->calls[i].before_rendering = NULL;
        model->calls[i].after_rendering = NULL;
        model->calls[i].before_free = NULL;
        model->calls[i].after_free = NULL;
    }
    model->call_ptr = 0;
    return GE_E_NONE;
}

int _ge_gles_model_update_indices(GEModel *model, void *data, size_t size) {
    /* See _ge_gles_modelarray_update in modelarray.c to understand why I added
     * this condition. */
    if(!model->updatable) return GE_E_IMMUTABLE;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->indices.vbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 size*ge_type_size[model->indices.type], data,
                 model->updatable ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return GE_E_NONE;
}

int _ge_gles_model_set_attr(GEModel *model, GEModelAttr *attr) {
    model->attr = attr;
    return GE_E_NONE;
}

void _ge_gles_model_render(GEModel *model) {
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
    size_t i;
    
    /* If the rendering attributes are not set, the model cannot be rendered */
    if(model->attr == NULL) return;
    
    for(i=0;i<GE_MODEL_INHERIT_MAX;i++){
        if(model->calls[i].before_rendering){
            model->calls[i].before_rendering((void*)model, model->attr,
                                             model->extra[i]);
        }
    }
    
    for(i=0;i<model->array_num;i++){
        if(model->arrays[i] == NULL || model->attr->array_pos[i] == NULL){
            continue;
        }
        ge_modelarray_enable(model->arrays[i], model->attr->array_pos[i]);
    }
    
    /* Bind the index array */
    if(model->indices.data){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->indices.vbo);
    }
    
    /* Draw the model */
    if(model->indices.data){
        glDrawElements(GL_TRIANGLES, model->indices.num,
                       gl_types[model->indices.type], 0);
    }else{
        glDrawArrays(GL_TRIANGLES, 0, model->indices.num);
    }
    
    /* Unbind the index buffer */
    if(model->indices.data) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    /* Unbind the buffers */
    for(i=0;i<model->array_num;i++){
        if(model->arrays[i] == NULL || model->attr->array_pos[i] == NULL){
            continue;
        }
        ge_modelarray_disable(model->arrays[i]);
    }
    
    for(i=0;i<GE_MODEL_INHERIT_MAX;i++){
        if(model->calls[i].after_rendering){
            model->calls[i].after_rendering((void*)model, model->attr,
                                            model->extra[i]);
        }
    }
}

void _ge_gles_model_render_multiple(GEModel *model, GEShaderPos **pos,
                                    GEUniformType *types, void **uniforms,
                                    size_t uniform_count, size_t count) {
    /* TODO: Use GL_EXT_draw_instanced if possible. */
    int uniform_sizes[GE_U_AMOUNT] = {
        sizeof(GEMat4),
        sizeof(GEMat3),
        sizeof(GEVec4),
        sizeof(GEVec3),
        sizeof(GEVec2)
    };
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
    size_t i, n;
    
    /* If the rendering attributes are not set, the model cannot be rendered */
    if(model->attr == NULL) return;
    
    for(i=0;i<GE_MODEL_INHERIT_MAX;i++){
        if(model->calls[i].before_rendering){
            model->calls[i].before_rendering((void*)model, model->attr,
                                             model->extra[i]);
        }
    }
    
    for(i=0;i<model->array_num;i++){
        if(model->arrays[i] == NULL || model->attr->array_pos[i] == NULL){
            continue;
        }
        ge_modelarray_enable(model->arrays[i], model->attr->array_pos[i]);
    }
    
    /* Bind the index array */
    if(model->indices.data){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->indices.vbo);
    }
    
    /* Draw the model multiple times */
    for(i=0;i<count;i++){
        /* Load all the uniform variables */
        for(n=0;n<uniform_count;n++){
            ge_shader_load_any(pos[n], types[n],
                               (char*)uniforms[n]+i*uniform_sizes[types[n]]);
        }
        if(model->indices.data){
            glDrawElements(GL_TRIANGLES, model->indices.num,
                           gl_types[model->indices.type], 0);
        }else{
            glDrawArrays(GL_TRIANGLES, 0, model->indices.num);
        }
    }
    
    /* Unbind the index buffer */
    if(model->indices.data) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    /* Unbind the buffers */
    for(i=0;i<model->array_num;i++){
        if(model->arrays[i] == NULL || model->attr->array_pos[i] == NULL){
            continue;
        }
        ge_modelarray_disable(model->arrays[i]);
    }
    
    for(i=0;i<GE_MODEL_INHERIT_MAX;i++){
        if(model->calls[i].after_rendering){
            model->calls[i].after_rendering((void*)model, model->attr,
                                            model->extra[i]);
        }
    }
}

int _ge_gles_model_attr_init(GEModelAttr *attr, GEShader *shader,
                             GEModelArrayAttr **array_attr, char **names,
                             size_t num) {
    size_t i;
    attr->array_pos = array_attr;
    attr->array_num = num;
    for(i=0;i<num;i++){
        if(!names[i] || !attr->array_pos[i]) continue;
        attr->array_pos[i]->pos = glGetAttribLocation(shader->shader_program,
                                                      names[i]);
    }
    return GE_E_NONE;
}

void _ge_gles_model_free(GEModel *model) {
    size_t i;
    
    for(i=0;i<GE_MODEL_INHERIT_MAX;i++){
        if(model->calls[i].before_free){
            model->calls[i].before_free((void*)model, model->extra[i]);
        }
    }
    
    for(i=0;i<model->array_num;i++){
        if(model->arrays[i] == NULL) continue;
        ge_modelarray_free(model->arrays[i]);
    }
    
    for(i=0;i<GE_MODEL_INHERIT_MAX;i++){
        if(model->calls[i].after_free){
            model->calls[i].after_free((void*)model, model->extra[i]);
        }
    }
}

