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

#include <mibiengine2/render2d/sprite.h>
#include <mibiengine2/errors.h>

int ge_sprite_init(GESprite *sprite, GETexture *texture, GEStdShader *shader,
                   float w, float h) {
    /* Model data */
    float vertices[4*2] = {
        0, 1,
        0, 0,
        1, 0,
        1, 1
    };
    unsigned short int indices[6] = {
        0, 1, 3,
        3, 1, 2
    };
    float uv_coords[4*2] = {
        0, 0,
        0, 1,
        1, 1,
        1, 0
    };
    char *attr_names[] = {
        GE_STDSHADER_VERTEX,
        GE_STDSHADER_COLOR,
        GE_STDSHADER_UV,
        GE_STDSHADER_NORMAL
    };
    
    size_t i;
    
    for(i=0;i<4*2;i+=2){
        vertices[i] *= w;
        vertices[i+1] *= h;
    }
    
    /* Initialize the model used to render the sprite */
    if(ge_texturedmodel_init(&sprite->model, texture, indices, vertices,
                             GE_T_USHORT, GE_T_FLOAT, 6, 4*2, 2, 0, NULL)){
        return GE_E_TEXTUREDMODEL_INIT;
    }
    if(ge_texturedmodel_set_texture(&sprite->model, &shader->texture,
                                    &shader->uv_max)){
        ge_model_free(&sprite->model);
        return GE_E_TEXTUREDMODEL_SET_TEX;
    }
    if(ge_stdmodel_add_uv_coords(&sprite->model, uv_coords, GE_T_FLOAT,
                                 4*2, 2)){
        ge_model_free(&sprite->model);
        return GE_E_STDMODEL_ADD;
    }
    if(ge_stdmodel_shader_attr(&sprite->model, shader->shader, attr_names)){
        ge_model_free(&sprite->model);
        return GE_E_STDMDOEL_SET_ATTR;
    }
    return GE_E_NONE;
}

void ge_sprite_free(GESprite *sprite) {
    ge_model_free(&sprite->model);
}

