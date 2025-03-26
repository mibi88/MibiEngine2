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

#include <mibiengine2/renderer/loader.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define _GE_LOADER_CPY_STR(dest, str) memcpy(dest, str, strlen(str)+1)

char *ge_loader_load_text(char *file, size_t *size_ptr) {
    FILE *fp;
    char *data;
    size_t size;
    
    fp = fopen(file, "r");
    if(fp == NULL){
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);
    data = malloc(size+1);
    if(data == NULL){
        fclose(fp);
        return NULL;
    }
    fread(data, 1, size, fp);
    
    fclose(fp);
    data[size] = '\0';
    if(size_ptr) *size_ptr = size;
    return data;
}

int ge_loader_load_obj(GEModel *model, GEShader *shader, GETexture *texture,
                       char *file, char **attr_names, char *tex_name) {
    void *data;
    size_t size;
    GEObj obj;
    GEShaderPos tex_pos;
    
    data = ge_loader_load_text(file, &size);
    if(data == NULL) return 1;
    
    if(ge_obj_init(&obj, data, size)){
        free(data);
        return 2;
    }
    
    if(texture == NULL){
        if(ge_stdmodel_init(model, obj.indices, obj.vertices,
                            GE_T_UINT, GE_T_FLOAT, obj.index_num,
                            obj.vertex_num, 4, NULL)){
            ge_obj_free(&obj);
            free(data);
            return 3;
        }
    }else{
        if(ge_texturedmodel_init(model, texture, obj.indices, obj.vertices,
                                 GE_T_UINT, GE_T_FLOAT, obj.index_num,
                                 obj.vertex_num, 4, NULL)){
            ge_obj_free(&obj);
            free(data);
            return 3;
        }
    }
    if(ge_stdmodel_add_uv_coords(model, obj.uv_coords, GE_T_FLOAT, obj.uv_num,
                                 3)){
        ge_model_free(model);
        ge_obj_free(&obj);
        free(data);
        return 4;
    }
    if(ge_stdmodel_add_normals(model, obj.normals, GE_T_FLOAT, obj.normal_num,
                              3)){
        ge_model_free(model);
        ge_obj_free(&obj);
        free(data);
        return 5;
    }
    if(ge_stdmodel_shader_attr(model, shader, attr_names)){
        ge_model_free(model);
        ge_obj_free(&obj);
        free(data);
        return 6;
    }
    tex_pos = ge_shader_get_pos(shader, tex_name);
    if(ge_texturedmodel_set_texture(model, &tex_pos)){
        ge_model_free(model);
        ge_obj_free(&obj);
        free(data);
        return 7;
    }
    ge_obj_free(&obj);
    free(data);
    return 0;
}

char *ge_loader_load_shader(GEShader *shader, char *vertex_file,
                            char *fragment_file) {
    char *vertex_shader;
    char *fragment_shader;
    char *log;
    
    vertex_shader = ge_loader_load_text(vertex_file, NULL);
    fragment_shader = ge_loader_load_text(fragment_file, NULL);
    
    if(vertex_shader == NULL || fragment_shader == NULL){
        free(vertex_shader);
        free(fragment_shader);
        return "Failed to load shaders!\n";
    }
    
    log = ge_shader_init(shader, vertex_shader, fragment_shader);
    free(vertex_shader);
    free(fragment_shader);
    return log;
}

