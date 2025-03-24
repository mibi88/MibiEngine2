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

#include <mibiengine2/base/obj.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

typedef struct {
    long vertex;
    long uv;
    long normal;
} GEObjIndexGroup;

#define _GE_OBJ_FREE() \
    { \
        ge_obj_free(obj); \
        free(vertices); \
        vertices = NULL; \
        free(uv); \
        uv = NULL; \
        free(normals); \
        normals = NULL; \
        free(indices); \
        indices = NULL; \
    }

#define _GE_OBJ_FREE_LOCAL() \
    { \
        free(vertices); \
        vertices = NULL; \
        free(uv); \
        uv = NULL; \
        free(normals); \
        normals = NULL; \
        free(indices); \
        indices = NULL; \
        free(vertices_used); \
        vertices_used = NULL; \
        free(new_index); \
        new_index = NULL; \
    }

#define _GE_OBJ_RESIZE(ptr, sz, max, type, err) \
    { \
        new = realloc(ptr, max*sizeof(type)); \
        if(new == NULL){ \
            err; \
        } \
        ptr = new; \
    }

#define _GE_OBJ_GROW_IF_NEEDED(ptr, sz, max, type, err) \
    { \
        if(sz >= max){ \
            max += GE_OBJ_ALLOC_STEP; \
            _GE_OBJ_RESIZE(ptr, sz, max, type, err); \
        } \
    }

#define _GE_OBJ_GROW(ptr, sz, max, type, err) \
    { \
        sz++; \
        _GE_OBJ_GROW_IF_NEEDED(ptr, sz, max, type, err); \
    }

#define _GE_OBJ_ATOI(s, n) sscanf(s, "%ld", &n)
#define _GE_OBJ_FTOI(s, n) sscanf(s, "%f", &n)
/* Convert vertex indices to GEObjIndexGroup */
#define _GE_OBJ_VTOG(s, g) \
    { \
        g.vertex = 0; \
        g.uv = 0; \
        g.normal = 0; \
        sscanf(s, "%ld/%ld/%ld", &g.vertex, &g.uv, &g.normal); \
        g.vertex--; \
        g.uv--; \
        g.normal--; \
    }

#define _GE_OBJ_ADD(ptr, sz, max, type, x) \
    { \
        ptr[sz] = x; \
        _GE_OBJ_GROW(ptr, sz, max, type, { \
            _GE_OBJ_FREE(); \
            _GE_OBJ_FREE_LOCAL(); \
            return 1; \
        }); \
    }

int ge_obj_init(GEObj *obj, char *data, size_t size) {
    /* TODO: Support relative indices */
    size_t i;
    size_t n;
    char line[GE_OBJ_TOK_MAX][GE_OBJ_TOK_SZ];
    int in_comment = 0;
    size_t line_len = 0;
    int in_data = 1;
    size_t token_pos = 0;
    char c;
    void *new;
    float len;
    
    float tmp;
    GEObjIndexGroup gtmp;
    
    float *vertices;
    float *uv;
    float *normals;
    GEObjIndexGroup *indices;
    
    char *vertices_used = NULL;
    size_t *new_index = NULL;
    
    size_t vertex_num = 0;
    size_t uv_num = 0;
    size_t normal_num = 0;
    size_t index_num = 0;
    
    size_t vertex_max_num = GE_OBJ_ALLOC_STEP;
    size_t uv_max_num = GE_OBJ_ALLOC_STEP;
    size_t normal_max_num = GE_OBJ_ALLOC_STEP;
    size_t index_max_num = GE_OBJ_ALLOC_STEP;
    
    int found;
    
    vertices = malloc(GE_OBJ_ALLOC_STEP*sizeof(float));
    uv = malloc(GE_OBJ_ALLOC_STEP*sizeof(float));
    normals = malloc(GE_OBJ_ALLOC_STEP*sizeof(float));
    indices = malloc(GE_OBJ_ALLOC_STEP*sizeof(GEObjIndexGroup));
    
    obj->vertices = malloc(GE_OBJ_ALLOC_STEP*sizeof(float));
    obj->uv_coords = malloc(GE_OBJ_ALLOC_STEP*sizeof(float));
    obj->normals = malloc(GE_OBJ_ALLOC_STEP*sizeof(float));
    obj->indices = malloc(GE_OBJ_ALLOC_STEP*sizeof(unsigned int));
    
    obj->vertex_num = 0;
    obj->uv_num = 0;
    obj->normal_num = 0;
    obj->index_num = 0;
    
    obj->vertex_max_num = GE_OBJ_ALLOC_STEP;
    obj->uv_max_num = GE_OBJ_ALLOC_STEP;
    obj->normal_max_num = GE_OBJ_ALLOC_STEP;
    obj->index_max_num = GE_OBJ_ALLOC_STEP;
    
    for(i=0;i<size;i++){
        c = data[i];
        if(c == '\n'){
            if(line_len || token_pos){
                if(in_data){
                    line[line_len][token_pos] = '\0';
                    line_len++;
                    if(line_len >= GE_OBJ_TOK_MAX) line_len = GE_OBJ_TOK_MAX-1;
                }
                /* Decode the line */
                if(!strcmp(line[0], "v")){
                    if(line_len == 4){
                        _GE_OBJ_FTOI(line[1], tmp);
                        _GE_OBJ_ADD(vertices, vertex_num, vertex_max_num,
                                    float, tmp);
                        _GE_OBJ_FTOI(line[2], tmp);
                        _GE_OBJ_ADD(vertices, vertex_num, vertex_max_num,
                                    float, tmp);
                        _GE_OBJ_FTOI(line[3], tmp);
                        _GE_OBJ_ADD(vertices, vertex_num, vertex_max_num,
                                    float, tmp);
                        _GE_OBJ_ADD(vertices, vertex_num, vertex_max_num,
                                    float, 1.0);
                    }else if(line_len == 5){
                        _GE_OBJ_FTOI(line[1], tmp);
                        _GE_OBJ_ADD(vertices, vertex_num, vertex_max_num,
                                    float, tmp);
                        _GE_OBJ_FTOI(line[2], tmp);
                        _GE_OBJ_ADD(vertices, vertex_num, vertex_max_num,
                                    float, tmp);
                        _GE_OBJ_FTOI(line[3], tmp);
                        _GE_OBJ_ADD(vertices, vertex_num, vertex_max_num,
                                    float, tmp);
                        _GE_OBJ_FTOI(line[4], tmp);
                        _GE_OBJ_ADD(vertices, vertex_num, vertex_max_num,
                                    float, tmp);
                    }else{
                        /* Invalid data */
#if GE_OBJ_DEBUG
                        fprintf(stderr, "Invalid vertex: length: %ld\n",
                                line_len);
#endif
                    }
                }else if(!strcmp(line[0], "vt")){
                    if(line_len == 2){
                        _GE_OBJ_FTOI(line[1], tmp);
                        _GE_OBJ_ADD(uv, uv_num, uv_max_num, float, tmp);
                        _GE_OBJ_ADD(uv, uv_num, uv_max_num, float, 1.0);
                        _GE_OBJ_ADD(uv, uv_num, uv_max_num, float, 1.0);
                    }else if(line_len == 3){
                        _GE_OBJ_FTOI(line[1], tmp);
                        _GE_OBJ_ADD(uv, uv_num, uv_max_num, float, tmp);
                        _GE_OBJ_FTOI(line[2], tmp);
                        _GE_OBJ_ADD(uv, uv_num, uv_max_num, float, tmp);
                        _GE_OBJ_ADD(uv, uv_num, uv_max_num, float, 1.0);
                    }else if(line_len == 4){
                        _GE_OBJ_FTOI(line[1], tmp);
                        _GE_OBJ_ADD(uv, uv_num, uv_max_num, float, tmp);
                        _GE_OBJ_FTOI(line[2], tmp);
                        _GE_OBJ_ADD(uv, uv_num, uv_max_num, float, tmp);
                        _GE_OBJ_FTOI(line[3], tmp);
                        _GE_OBJ_ADD(uv, uv_num, uv_max_num, float, tmp);
                    }else{
                        /* Invalid data */
#if GE_OBJ_DEBUG
                        fprintf(stderr, "Invalid uv coordinate: length: %ld\n",
                                line_len);
#endif
                    }
                }else if(!strcmp(line[0], "vn")){
                    if(line_len == 4){
                        _GE_OBJ_FTOI(line[1], tmp);
                        _GE_OBJ_ADD(normals, normal_num, normal_max_num,
                                    float, tmp);
                        _GE_OBJ_FTOI(line[2], tmp);
                        _GE_OBJ_ADD(normals, normal_num, normal_max_num,
                                    float, tmp);
                        _GE_OBJ_FTOI(line[3], tmp);
                        _GE_OBJ_ADD(normals, normal_num, normal_max_num,
                                    float, tmp);
                    }else{
                        /* Invalid data */
#if GE_OBJ_DEBUG
                        fprintf(stderr, "Invalid normal: length: %ld\n",
                                line_len);
#endif
                    }
                }else if(!strcmp(line[0], "vp")){
                    /* TODO */
#if GE_OBJ_DEBUG
                    fputs("Parameter space vertices are currently "
                          "unsupported!", stderr);
#endif
                }else if(!strcmp(line[0], "f")){
                    /* TODO: Support other shapes than triangles */
#if GE_OBJ_DEBUG
                    if(line_len > 4){
                        fputs("Other shapes than triangles are currently "
                              "unsupported!", stderr);
                    }else if(line_len == 4){
#else
                    if(line_len == 4){
#endif
                        /* Load the triangle */
                        _GE_OBJ_VTOG(line[1], gtmp);
                        _GE_OBJ_ADD(indices, index_num, index_max_num,
                                    GEObjIndexGroup, gtmp);
                        _GE_OBJ_VTOG(line[2], gtmp);
                        _GE_OBJ_ADD(indices, index_num, index_max_num,
                                    GEObjIndexGroup, gtmp);
                        _GE_OBJ_VTOG(line[3], gtmp);
                        _GE_OBJ_ADD(indices, index_num, index_max_num,
                                    GEObjIndexGroup, gtmp);
                    }
                }else if(!strcmp(line[0], "l")){
                    /* TODO */
#if GE_OBJ_DEBUG
                    fputs("Lines are currently unsupported!", stderr);
#endif
                }else{
#if GE_OBJ_DEBUG
                    fprintf(stderr, "Unknown line starting with \"%s\"!\n",
                            line[0]);
#endif
                }
            }
            /* Reset the position in the line */
            line_len = 0;
            token_pos = 0;
            /* We're not in a comment anymore, if we were in a comment in the
             * first place. */
            in_comment = 0;
            continue;
        }
        if(!in_comment){
            if(c == '#'){
                in_comment = 1;
                continue;
            }
            if(c < 0x21 || c > 0x7e){
                if(in_data && token_pos){
                  line[line_len][token_pos] = '\0';
                  line_len++;
                  if(line_len >= GE_OBJ_TOK_MAX) line_len = GE_OBJ_TOK_MAX-1;
                }
                token_pos = 0;
                in_data = 0;
                continue;
            }
            line[line_len][token_pos] = c;
            token_pos++;
            if(token_pos >= GE_OBJ_TOK_SZ) token_pos = GE_OBJ_TOK_SZ-1;
            in_data = 1;
        }
    }
    /* Normalize the normals */
    n = normal_num/3;
    for(i=0;i<n*3;i+=3){
        len = sqrt(normals[i]*normals[i]+normals[i+1]*normals[i+1]+
                   normals[i+2]*normals[i+2]);
        normals[i] /= len;
        normals[i+1] /= len;
        normals[i+2] /= len;
    }
    /* Generate the arrays in the GEObj struct */
    /* TODO: Optimize this! */
    vertices_used = calloc(vertex_num/4, 1);
    new_index = calloc(index_num, sizeof(size_t));
    if(vertices_used == NULL || new_index == NULL){
        _GE_OBJ_FREE();
        _GE_OBJ_FREE_LOCAL();
        return 2;
    }
    for(i=0;i<index_num;i++){
        /* Fix the indices if they are incorrect */
        if((size_t)indices[i].vertex >= vertex_num/4){
#if GE_OBJ_DEBUG
            fprintf(stderr, "Bad vertex index %ld!\n", indices[i].vertex);
#endif
            indices[i].vertex = 0;
        }
        if((size_t)indices[i].uv >= uv_num/3){
#if GE_OBJ_DEBUG
            fprintf(stderr, "Bad uv coord index %ld!\n", indices[i].uv);
#endif
            indices[i].uv = 0;
        }
        if((size_t)indices[i].normal >= normal_num/3){
#if GE_OBJ_DEBUG
            fprintf(stderr, "Bad normal index %ld!\n", indices[i].normal);
#endif
            indices[i].normal = 0;
        }
        /* If this vertice was already used, check if it must be added because
         * it is using different normal or uv indices than the previous one */
        if(vertices_used[indices[i].vertex]){
            /* Search the previously added vertex or vertices with the same
             * vertex index */
            found = 0;
            for(n=0;n<i;n++){
                if(indices[n].vertex == indices[i].vertex &&
                   indices[n].uv == indices[i].uv &&
                   indices[n].normal == indices[i].normal){
                    found = 1;
                    break;
                }
            }
            if(found){
                _GE_OBJ_ADD(obj->indices, obj->index_num, obj->index_max_num,
                            float, new_index[n]);
                new_index[i] = new_index[n];
            }else{
                /* Add the vertex and its index */
                _GE_OBJ_ADD(obj->indices, obj->index_num, obj->index_max_num,
                            float, obj->vertex_num/4);
                new_index[i] = obj->vertex_num/4;
                _GE_OBJ_ADD(obj->vertices, obj->vertex_num,
                            obj->vertex_max_num, float,
                            vertices[indices[i].vertex*4]);
                _GE_OBJ_ADD(obj->vertices, obj->vertex_num,
                            obj->vertex_max_num, float,
                            vertices[indices[i].vertex*4+1]);
                _GE_OBJ_ADD(obj->vertices, obj->vertex_num,
                            obj->vertex_max_num, float,
                            vertices[indices[i].vertex*4+2]);
                _GE_OBJ_ADD(obj->vertices, obj->vertex_num,
                            obj->vertex_max_num, float,
                            vertices[indices[i].vertex*4+3]);
                _GE_OBJ_ADD(obj->uv_coords, obj->uv_num, obj->uv_max_num, float,
                            uv[indices[i].uv*3]);
                _GE_OBJ_ADD(obj->uv_coords, obj->uv_num, obj->uv_max_num, float,
                            uv[indices[i].uv*3+1]);
                _GE_OBJ_ADD(obj->uv_coords, obj->uv_num, obj->uv_max_num, float,
                            uv[indices[i].uv*3+2]);
                _GE_OBJ_ADD(obj->normals, obj->normal_num, obj->normal_max_num,
                            float, normals[indices[i].normal*3]);
                _GE_OBJ_ADD(obj->normals, obj->normal_num, obj->normal_max_num,
                            float, normals[indices[i].normal*3+1]);
                _GE_OBJ_ADD(obj->normals, obj->normal_num, obj->normal_max_num,
                            float, normals[indices[i].normal*3+2]);
            }
        }else{
            /* Add the vertex and its index */
            _GE_OBJ_ADD(obj->indices, obj->index_num, obj->index_max_num,
                        float, obj->vertex_num/4);
            new_index[i] = obj->vertex_num/4;
            _GE_OBJ_ADD(obj->vertices, obj->vertex_num,
                        obj->vertex_max_num, float,
                        vertices[indices[i].vertex*4]);
            _GE_OBJ_ADD(obj->vertices, obj->vertex_num,
                        obj->vertex_max_num, float,
                        vertices[indices[i].vertex*4+1]);
            _GE_OBJ_ADD(obj->vertices, obj->vertex_num,
                        obj->vertex_max_num, float,
                        vertices[indices[i].vertex*4+2]);
            _GE_OBJ_ADD(obj->vertices, obj->vertex_num,
                        obj->vertex_max_num, float,
                        vertices[indices[i].vertex*4+3]);
            _GE_OBJ_ADD(obj->uv_coords, obj->uv_num, obj->uv_max_num, float,
                        uv[indices[i].uv*3]);
            _GE_OBJ_ADD(obj->uv_coords, obj->uv_num, obj->uv_max_num, float,
                        uv[indices[i].uv*3+1]);
            _GE_OBJ_ADD(obj->uv_coords, obj->uv_num, obj->uv_max_num, float,
                        uv[indices[i].uv*3+2]);
            _GE_OBJ_ADD(obj->normals, obj->normal_num, obj->normal_max_num,
                        float, normals[indices[i].normal*3]);
            _GE_OBJ_ADD(obj->normals, obj->normal_num, obj->normal_max_num,
                        float, normals[indices[i].normal*3+1]);
            _GE_OBJ_ADD(obj->normals, obj->normal_num, obj->normal_max_num,
                        float, normals[indices[i].normal*3+2]);
            /* The vertex has been used */
            vertices_used[indices[i].vertex] = 1;
        }
    }
#if GE_OBJ_DEBUG
    printf("-- INDICES LOADED: %ld\n", obj->index_num);
    printf("-- VERTICES LOADED: %ld\n", obj->vertex_num);
    printf("-- UV COORDINATES LOADED: %ld\n", obj->uv_num);
    printf("-- NORMALS LOADED: %ld\n", obj->normal_num);
#endif
    /* Free all the variables */
    _GE_OBJ_FREE_LOCAL();
#if GE_OBJ_DEBUG
    puts("-- MODEL LOADED SUCCESSFULLY");
#endif
    return 0;
}

void ge_obj_free(GEObj *obj) {
    free(obj->vertices);
    free(obj->uv_coords);
    free(obj->normals);
    free(obj->indices);
    obj->vertices = NULL;
    obj->uv_coords = NULL;
    obj->normals = NULL;
    obj->indices = NULL;
}

