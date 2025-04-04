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

#include <mibiengine2/errors.h>
#include <mibiengine2/render2d/tilemap.h>

#include <stdlib.h>

int _ge_tilemap_generate(GETilemap *tilemap, unsigned short int *tiles,
                         int size) {
    /* Quad data */
    unsigned char vertices[4*2] = {
        0, 1,
        0, 0,
        1, 0,
        1, 1
    };
    unsigned char uv_coords[4*2] = {
        0, 0,
        0, 1,
        1, 1,
        1, 0
    };
    unsigned short int indices[6] = {
        0, 1, 3,
        3, 1, 2
    };
    
    size_t i;
    int x, y, n;
    int tx, ty;
    
    float tile_w = 1/(float)tilemap->tiles_w;
    float tile_h = 1/(float)tilemap->tiles_h;
    
    free(tilemap->vertices);
    free(tilemap->uv_coords);
    free(tilemap->indices);
    
    tilemap->vertices = malloc(tilemap->w*tilemap->h*2*4*sizeof(float));
    tilemap->uv_coords = malloc(tilemap->w*tilemap->h*2*4*sizeof(float));
    tilemap->indices = malloc(tilemap->w*tilemap->h*6*sizeof(unsigned short));
    
    if(tilemap->vertices == NULL || tilemap->uv_coords == NULL ||
       tilemap->indices == NULL){
        free(tilemap->vertices);
        tilemap->vertices = NULL;
        free(tilemap->uv_coords);
        tilemap->uv_coords = NULL;
        free(tilemap->indices);
        tilemap->indices = NULL;
        return GE_E_OUT_OF_MEM;
    }
    
    i = 0;
    for(y=0;y<tilemap->h;y++){
        for(x=0;x<tilemap->h;x++){
            tx = tiles[i]%tilemap->tiles_w;
            ty = tiles[i]/tilemap->tiles_w;
            for(n=0;n<4*2;n++){
                if(n&1){
                    /* It's the Y axis */
                    tilemap->vertices[i*4*2+n] = -(y+vertices[n])*size;
                    tilemap->uv_coords[i*4*2+n] = (ty+uv_coords[n])*tile_h;
                }else{
                    /* It's the X axis */
                    tilemap->vertices[i*4*2+n] = -(x+vertices[n])*size;
                    tilemap->uv_coords[i*4*2+n] = (tx+uv_coords[n])*tile_w;
                }
            }
            for(n=0;n<6;n++){
                tilemap->indices[i*6+n] = i*4+indices[n];
            }
            i++;
        }
    }
    
    return GE_E_NONE;
}

int ge_tilemap_init(GETilemap *tilemap, GEImage *tileset, GETexture *texture,
                    GEStdShader *shader, unsigned short int *tiles, int w,
                    int h, int tiles_w, int tiles_h, int size) {
    int rc;
    char *attr_names[] = {
        GE_STDSHADER_VERTEX,
        GE_STDSHADER_COLOR,
        GE_STDSHADER_UV,
        GE_STDSHADER_NORMAL
    };
    
    tilemap->tiles = tiles;
    
    tilemap->tiles_w = tiles_w;
    tilemap->tiles_h = tiles_h;
    
    tilemap->w = w;
    tilemap->h = h;
    
    tilemap->tileset = tileset;
    
    tilemap->vertices = NULL;
    tilemap->uv_coords = NULL;
    tilemap->indices = NULL;
    
    rc = _ge_tilemap_generate(tilemap, tiles, size);
    if(rc) return rc;
    
    if(ge_texturedmodel_init(&tilemap->model, texture, tilemap->indices,
                             tilemap->vertices, GE_T_USHORT, GE_T_FLOAT, w*h*6,
                             w*h*4*2, 2, 1, NULL)){
        return GE_E_TEXTUREDMODEL_INIT;
    }
    if(ge_texturedmodel_set_texture(&tilemap->model, &shader->texture,
                                    &shader->uv_max)){
        ge_model_free(&tilemap->model);
        return GE_E_TEXTUREDMODEL_SET_TEX;
    }
    if(ge_stdmodel_add_uv_coords(&tilemap->model, tilemap->uv_coords,
                                 GE_T_FLOAT, w*h*4*2, 2)){
        ge_model_free(&tilemap->model);
        return GE_E_STDMODEL_ADD;
    }
    if(ge_stdmodel_shader_attr(&tilemap->model, shader->shader, attr_names)){
        ge_model_free(&tilemap->model);
        return GE_E_STDMDOEL_SET_ATTR;
    }
    return GE_E_NONE;
}

int ge_tilemap_update(GETilemap *tilemap, unsigned short int *tiles, int w,
                      int h, int size) {
    int rc;
    tilemap->tiles = tiles;
    tilemap->w = w;
    tilemap->h = h;
    
    rc = _ge_tilemap_generate(tilemap, tiles, size);
    if(rc) return rc;
    
    if(ge_model_update_indices(&tilemap->model, tilemap->indices, w*h*6)){
        return GE_E_MODEL_UPDATE_INDICES;
    }
    if(ge_stdmodel_update_vertices(&tilemap->model, tilemap->vertices,
                                   w*h*4*2)){
        return GE_E_STDMODEL_UPDATE_ARRAY;
    }
    if(ge_stdmodel_update_uv_coords(&tilemap->model, tilemap->uv_coords,
                                    w*h*4*2)){
        return GE_E_STDMODEL_UPDATE_ARRAY;
    }
    return GE_E_NONE;
}

void ge_tilemap_free(GETilemap *tilemap) {
    free(tilemap->vertices);
    tilemap->vertices = NULL;
    free(tilemap->uv_coords);
    tilemap->uv_coords = NULL;
    free(tilemap->indices);
    tilemap->indices = NULL;
}

