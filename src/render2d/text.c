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
#include <mibiengine2/render2d/text.h>

#include <stdlib.h>
#include <string.h>

int _ge_text_generate_arrays(GEText *text, GEFont *font, char *str,
                             size_t len, float size) {
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
    
    size_t i, n;
    int x, y;
    GEGlyph *glyph;
    
    text->vertices = malloc(len*4*2*sizeof(float));
    text->uv_coords = malloc(len*4*2*sizeof(float));
    text->indices = malloc(len*6*sizeof(unsigned short int));
    if(text->vertices == NULL || text->uv_coords == NULL ||
       text->indices == NULL){
        free(text->vertices);
        text->vertices = NULL;
        free(text->uv_coords);
        text->uv_coords = NULL;
        free(text->indices);
        text->indices = NULL;
        return GE_E_OUT_OF_MEM;
    }
    
    /* Create the arrays */
    text->w = 0;
    text->h = 0;
    x = 0;
    y = 0;
    for(i=0;i<len;i++){
        glyph = font->glyphs+font->glyph_pos(font, str[i]);
        for(n=0;n<4*2;n++){
            if(n&1){
                /* It's the position on the Y axis */
                if(vertices[n]){
                    text->vertices[i*4*2+n] = -y*size+font->line_height-
                                              glyph->y2*size;
                }else{
                    text->vertices[i*4*2+n] = -y*size+font->line_height-
                                              glyph->y1*size;
                }
                if(uv_coords[n]) text->uv_coords[i*4*2+n] = glyph->v2;
                else text->uv_coords[i*4*2+n] = glyph->v1;
            }else{
                /* It's the position on the X axis */
                if(vertices[n]) text->vertices[i*4*2+n] = -x*size+glyph->x2*
                                                          size;
                else text->vertices[i*4*2+n] = -x*size+glyph->x1*size;
                if(uv_coords[n]) text->uv_coords[i*4*2+n] = glyph->u2;
                else text->uv_coords[i*4*2+n] = glyph->u1;
            }
        }
        for(n=0;n<6;n++){
            text->indices[i*6+n] = indices[n]+i*4;
        }
        font->glyph_move_cursor(font, glyph-font->glyphs, &x, &y);
        if(x > text->w) text->w = x;
        if(y > text->h) text->h = x;
    }
    return GE_E_NONE;
}

int ge_text_init(GEText *text, GEFont *font, GETexture *texture,
                 GEStdShader *shader, char *str, float size) {
    size_t len;
    int rc;
    
    char *attr_names[] = {
        GE_STDSHADER_VERTEX,
        GE_STDSHADER_COLOR,
        GE_STDSHADER_UV,
        GE_STDSHADER_NORMAL
    };
    
    text->font = font;
    text->size = size;
    
    text->vertices = NULL;
    text->uv_coords = NULL;
    text->indices = NULL;
    
    len = strlen(str);
    
    rc = _ge_text_generate_arrays(text, font, str, len, size);
    if(rc) return rc;
    
    if(ge_texturedmodel_init(&text->model, texture, text->indices,
                             text->vertices, GE_T_USHORT, GE_T_FLOAT, len*6,
                             len*4*2, 2, 1, NULL)){
        return GE_E_TEXTUREDMODEL_INIT;
    }
    if(ge_texturedmodel_set_texture(&text->model, &shader->texture,
                                    &shader->uv_max)){
        ge_model_free(&text->model);
        return GE_E_TEXTUREDMODEL_SET_TEX;
    }
    if(ge_stdmodel_add_uv_coords(&text->model, text->uv_coords, GE_T_FLOAT,
                                 len*4*2, 2)){
        ge_model_free(&text->model);
        return GE_E_STDMODEL_ADD;
    }
    if(ge_stdmodel_shader_attr(&text->model, shader->shader, attr_names)){
        ge_model_free(&text->model);
        return GE_E_STDMDOEL_SET_ATTR;
    }
    return GE_E_NONE;
}

int ge_text_update(GEText *text, char *str) {
    size_t len;
    int rc;
    
    len = strlen(str);
    
    rc = _ge_text_generate_arrays(text, text->font, str, len, text->size);
    if(rc) return rc;
    
    if(ge_model_update_indices(&text->model, text->indices, len*6)){
        return GE_E_MODEL_UPDATE_INDICES;
    }
    if(ge_stdmodel_update_vertices(&text->model, text->vertices, len*4*2)){
        return GE_E_STDMODEL_UPDATE_ARRAY;
    }
    if(ge_stdmodel_update_uv_coords(&text->model, text->uv_coords, len*4*2)){
        return GE_E_STDMODEL_UPDATE_ARRAY;
    }
    
    return GE_E_NONE;
}

void ge_text_free(GEText *text) {
    free(text->vertices);
    text->vertices = NULL;
    free(text->uv_coords);
    text->uv_coords = NULL;
    free(text->indices);
    text->indices = NULL;
    /* The model should be freed separately if needed */
}

