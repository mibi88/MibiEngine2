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

#include <obj.h>

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
    }

#define _GE_OBJ_ADD(ptr, sz, max, type, x) \
    { \
        ptr[sz] = x; \
        _GE_OBJ_GROW(ptr, sz, max, type, { \
            _GE_OBJ_FREE(); \
            free(vertices); \
            vertices = NULL; \
            free(uv); \
            uv = NULL; \
            free(normals); \
            normals = NULL; \
            free(indices); \
            indices = NULL; \
            return 1; \
        }); \
    }

int ge_obj_load(GEObj *obj, char *data, size_t size) {
    /* TODO: Support relative indices */
    size_t i;
    size_t n;
    char *last;
    size_t count;
    char line[GE_OBJ_TOK_MAX][GE_OBJ_TOK_SZ];
    int in_comment = 0;
    size_t line_len = 0;
    int in_data = 1;
    size_t token_pos = 0;
    char c;
    unsigned int *uv_indices;
    unsigned int *normal_indices;
    void *new;
    float len;
    
    float tmp;
    GEObjIndexGroup gtmp;
    
    float *vertices;
    float *uv;
    float *normals;
    GEObjIndexGroup *indices;
    
    size_t vertex_num = 0;
    size_t uv_num = 0;
    size_t normal_num = 0;
    size_t index_num;
    
    size_t vertex_max_num = GE_OBJ_ALLOC_STEP;
    size_t uv_max_num = GE_OBJ_ALLOC_STEP;
    size_t normal_max_num = GE_OBJ_ALLOC_STEP;
    size_t index_max_num = GE_OBJ_ALLOC_STEP;
    
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
                        printf("Invalid vertex: length: %ld\n", line_len);
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
                        printf("Invalid uv coordinate: length: %ld\n",
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
                        printf("Invalid normal: length: %ld\n", line_len);
#endif
                    }
                }else if(!strcmp(line[0], "vp")){
                    /* TODO */
#if GE_OBJ_DEBUG
                    puts("Parameter space vertices are currently "
                         "unsupported!");
#endif
                }else if(!strcmp(line[0], "f")){
                    /* TODO: Support other shapes than triangles */
#if GE_OBJ_DEBUG
                    if(line_len > 4){
                        puts("Other shapes than triangles are currently "
                             "unsupported!");
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
                    puts("Lines are currently unsupported!");
#endif
                }else{
#if GE_OBJ_DEBUG
                    printf("Unknown line starting with \"%s\"!\n", line[0]);
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
#if GE_OBJ_DEBUG
    printf("-- INDICES LOADED: %ld\n", obj->index_num);
    printf("-- VERTICES LOADED: %ld\n", obj->vertex_num);
    printf("-- UV COORDINATES LOADED: %ld\n", obj->uv_num);
    printf("-- NORMALS LOADED: %ld\n", obj->normal_num);
#endif
    /* Normalize the normals */
    n = obj->normal_num/3;
    if(obj->normal_num%3) n--;
    for(i=0;i<obj->normal_num*3;i+=3){
        len = sqrt(obj->normals[i]*obj->normals[i]+
                   obj->normals[i+1]*obj->normals[i+1]+
                   obj->normals[i+2]*obj->normals[i+2]);
        obj->normals[i] = obj->normals[i]/len;
        obj->normals[i+1] = obj->normals[i+1]/len;
        obj->normals[i+2] = obj->normals[i+2]/len;
    }
    /* Free all the variables */
    free(vertices);
    free(uv);
    free(normals);
    free(indices);
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

