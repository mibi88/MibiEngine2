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

/* TODO: Clean this up! */

#if GE_OBJ_DEBUG_TRIANGLES
#define _GE_OBJ_DISPLAY_STR(str, len) \
    fwrite(str, 1, len, stdout); \
    puts("")
#else
#define _GE_OBJ_DISPLAY_STR(str, len)
#endif

#if GE_OBJ_DEBUG
#define _GE_OBJ_PRINT_DEBUG(s) puts(s)
#else
#define _GE_OBJ_PRINT_DEBUG(s)
#endif

#if GE_OBJ_DEBUG_INDICES
#define _GE_OBJ_PRINT_MOVE(ptr) \
    printf("Moved %d to %d\n", ptr[i], obj->indices[i]);
#else
#define _GE_OBJ_PRINT_MOVE(ptr)
#endif

#define _GE_OBJ_RESIZE(ptr, sz, max, t, err) \
    new = realloc(ptr, max*sizeof(t)); \
    if(new == NULL){ \
        _GE_OBJ_FREEALL(); \
        return err; \
    } \
    ptr = new

#define _GE_OBJ_GROW_IF_NEEDED(ptr, sz, max, t, err) \
    if(sz >= max){ \
        max += GE_OBJ_ALLOC_STEP; \
        _GE_OBJ_RESIZE(ptr, sz, max, t, err); \
    }

#define _GE_OBJ_GROW(ptr, sz, max, t, err) \
    sz++; \
    _GE_OBJ_GROW_IF_NEEDED(ptr, sz, max, t, err);

#define _GE_OBJ_FREEOBJ() \
    free(obj->vertices); \
    free(obj->uv_coords); \
    free(obj->normals); \
    free(obj->indices); \
    obj->vertices = NULL; \
    obj->uv_coords = NULL; \
    obj->normals = NULL; \
    obj->indices = NULL

#define _GE_OBJ_FREEALL() \
    free(uv_indices); \
    free(normal_indices); \
    uv_indices = NULL; \
    normal_indices = NULL; \
    _GE_OBJ_FREEOBJ();

#define _GE_OBJ_LOAD_SHAPE(x) \
    count = 0; \
    last = line[x]; \
    for(n=0;n<strlen(line[x])+1;n++) { \
        if(line[x][n] == '/' || line[x][n] == '\0'){ \
            _GE_OBJ_DISPLAY_STR(last, line[x]+n-last); \
            switch(count){ \
                case 0: \
                    /* It's a vertex index */ \
                    obj->indices[obj->index_num] = \
                            _ge_atoi(last, line[x]+n-last)-1; \
                    break; \
                case 1: \
                    /* It's a uv coordinate index */ \
                    uv_indices[obj->index_num] = \
                            _ge_atoi(last, line[x]+n-last)-1; \
                    break; \
                case 2: \
                    /* It's a uv coordinate index */ \
                    normal_indices[obj->index_num] = \
                            _ge_atoi(last, line[x]+n-last)-1; \
                    break; \
            } \
            last = line[x]+n+1; \
            count++; \
        } \
    } \
    if(count < 3) normal_indices[obj->index_num] = 0; \
    if(count < 2) uv_indices[obj->index_num] = 0; \
    obj->index_num++; \
    if(obj->index_num >= obj->index_max_num){ \
        obj->index_max_num += GE_OBJ_ALLOC_STEP; \
        _GE_OBJ_RESIZE(obj->indices, obj->index_num, \
                       obj->index_max_num, unsigned int, 1); \
        _GE_OBJ_RESIZE(uv_indices, obj->index_num, \
                       obj->index_max_num, unsigned int, 1); \
        _GE_OBJ_RESIZE(normal_indices, obj->index_num, \
                       obj->index_max_num, unsigned int, 1); \
    }

#define _GE_OBJ_LOAD(ptr, sz, max, x) \
    sscanf(line[x], "%f", ptr+sz); \
    _GE_OBJ_GROW(ptr, sz, max, float, 2);

#define _GE_OBJ_ADD(ptr, sz, max, x) \
    ptr[sz] = x; \
    _GE_OBJ_GROW(ptr, sz, max, float, 2);

#define _GE_OBJ_SORT_INDICES(ptr, data, sz, num, t, err) \
    new = malloc(num*sz*sizeof(t)); \
    if(new == NULL){ \
        _GE_OBJ_FREEALL(); \
        return err; \
    } \
    memset(new, 0, num*sz*sizeof(t)); \
    for(i=0;i<obj->index_num;i++){ \
        if(ptr[i] >= num/sz || obj->indices[i] >= num/sz){ \
            _GE_OBJ_PRINT_DEBUG("Bad index!"); \
            continue; \
        } \
        _GE_OBJ_PRINT_MOVE(ptr) \
        memcpy(((float*)new)+obj->indices[i]*sz, data+ptr[i]*sz, \
               sz*sizeof(t)); \
    } \
    free(data); \
    data = new;

int _ge_atoi(char *str, size_t size) {
    size_t i;
    int n = 0;
#if GE_OBJ_DEBUG_ATOI
    fputs("STR: ", stdout);
    fwrite(str, 1, size, stdout);
    puts("");
#endif
    for(i=0;i<size;i++){
        if(str[i] >= '0' && str[i] <= '9'){
            n *= 10;
            n += str[i]-'0';
        }
    }
#if GE_OBJ_DEBUG_ATOI
    printf("NUM: %d\n", n);
#endif
    return n;
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
    
    uv_indices = malloc(GE_OBJ_ALLOC_STEP*sizeof(unsigned int));
    normal_indices = malloc(GE_OBJ_ALLOC_STEP*sizeof(unsigned int));
    
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
#if GE_OBJ_DEBUG_LINES
                printf("-- LINE: %ld\n", line_len);
                for(n=0;n<line_len;n++) printf("%s ", line[n]);
                puts("");
#endif
                if(!strcmp(line[0], "v")){
                    if(line_len == 4){
                        _GE_OBJ_LOAD(obj->vertices, obj->vertex_num,
                                     obj->vertex_max_num, 1);
                        _GE_OBJ_LOAD(obj->vertices, obj->vertex_num,
                                     obj->vertex_max_num, 2);
                        _GE_OBJ_LOAD(obj->vertices, obj->vertex_num,
                                     obj->vertex_max_num, 3);
                        _GE_OBJ_ADD(obj->vertices, obj->vertex_num,
                                        obj->vertex_max_num, 1.0);
                    }else if(line_len == 5){
                        _GE_OBJ_LOAD(obj->vertices, obj->vertex_num,
                                     obj->vertex_max_num, 1);
                        _GE_OBJ_LOAD(obj->vertices, obj->vertex_num,
                                     obj->vertex_max_num, 2);
                        _GE_OBJ_LOAD(obj->vertices, obj->vertex_num,
                                     obj->vertex_max_num, 3);
                        _GE_OBJ_LOAD(obj->vertices, obj->vertex_num,
                                     obj->vertex_max_num, 4);
                    }else{
                        /* Invalid data */
#if GE_OBJ_DEBUG
                        printf("Invalid vertex: length: %ld\n", line_len);
#endif
                    }
                }else if(!strcmp(line[0], "vt")){
                    if(line_len == 2){
                        _GE_OBJ_LOAD(obj->uv_coords, obj->uv_num,
                                     obj->uv_max_num, 1);
                        _GE_OBJ_ADD(obj->uv_coords, obj->uv_num,
                                    obj->uv_max_num, 0.0);
                        _GE_OBJ_ADD(obj->uv_coords, obj->uv_num,
                                    obj->uv_max_num, 0.0);
                    }else if(line_len == 3){
                        _GE_OBJ_LOAD(obj->uv_coords, obj->uv_num,
                                     obj->uv_max_num, 1);
                        _GE_OBJ_LOAD(obj->uv_coords, obj->uv_num,
                                     obj->uv_max_num, 2);
                        _GE_OBJ_ADD(obj->uv_coords, obj->uv_num,
                                    obj->uv_max_num, 0.0);
                    }else if(line_len == 4){
                        _GE_OBJ_LOAD(obj->uv_coords, obj->uv_num,
                                     obj->uv_max_num, 1);
                        _GE_OBJ_LOAD(obj->uv_coords, obj->uv_num,
                                     obj->uv_max_num, 2);
                        _GE_OBJ_LOAD(obj->uv_coords, obj->uv_num,
                                     obj->uv_max_num, 3);
                    }else{
                        /* Invalid data */
#if GE_OBJ_DEBUG
                        printf("Invalid uv coordinate: length: %ld\n",
                               line_len);
#endif
                    }
                }else if(!strcmp(line[0], "vn")){
                    if(line_len == 4){
                        _GE_OBJ_LOAD(obj->normals, obj->normal_num,
                                     obj->normal_max_num, 1);
                        _GE_OBJ_LOAD(obj->normals, obj->normal_num,
                                     obj->normal_max_num, 2);
                        _GE_OBJ_LOAD(obj->normals, obj->normal_num,
                                     obj->normal_max_num, 3);
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
#if GE_OBJ_DEBUG_TRIANGLES
                        puts("-- TRIANGLE");
#endif
                        _GE_OBJ_LOAD_SHAPE(1);
                        _GE_OBJ_LOAD_SHAPE(2);
                        _GE_OBJ_LOAD_SHAPE(3);
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
    /* Sort the normals and uv coordinates depending on the indices and the
     * uv coordinates indices. */
    /* for(i=0;i<obj->index_num;i++) printf("%d ", normal_indices[i]);
    puts(""); */
    _GE_OBJ_SORT_INDICES(uv_indices, obj->uv_coords, 3, obj->uv_num, float, 3);
    _GE_OBJ_SORT_INDICES(normal_indices, obj->normals, 3, obj->normal_num,
                         float, 3);
    /* Normalize the normals */
    n = obj->normal_num/3;
    if(obj->normal_num%3) n--;
    for(i=0;i<obj->normal_num*3;i+=3){
        len = sqrt(obj->normals[i]*obj->normals[i]+
                   obj->normals[i+1]*obj->normals[i+1]+
                   obj->normals[i+2]*obj->normals[i+2]);
#if GE_OBJ_DEBUG_NORMALIZE
        printf("Length of the normal: %f\n", len);
#endif
        obj->normals[i] = obj->normals[i]/len;
        obj->normals[i+1] = obj->normals[i+1]/len;
        obj->normals[i+2] = obj->normals[i+2]/len;
    }
    /* Free the uv and normal indices */
    free(uv_indices);
    free(normal_indices);
#if GE_OBJ_DEBUG
    puts("-- MODEL LOADED SUCCESSFULLY");
#endif
    return 0;
}

void ge_obj_free(GEObj *obj) {
    _GE_OBJ_FREEOBJ();
}

