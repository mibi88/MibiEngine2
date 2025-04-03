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

#include <mibiengine2/base/texturedmodel.h>

#include <mibiengine2/errors.h>

#include <stdlib.h>

void _ge_texturedmodel_before_rendering(void *model, GEModelAttr *attr,
                                        void *extra) {
    GETexturedModel *texturedmodel = extra;
    (void)model;
    (void)attr;
    if(texturedmodel->uv_max_pos){
        ge_shader_load_vec2(texturedmodel->uv_max_pos,
                            &texturedmodel->texture->uv_max);
    }
    if(texturedmodel->tex_pos){
        /* Use the first texture unit */
        ge_texture_use(texturedmodel->texture, texturedmodel->tex_pos, 0);
    }
}

void _ge_textured_after_free(void *_model, void *extra) {
    GEModel *model = _model;
    free(extra);
    model->extra[GE_TEXTUREDMODEL_INHERIT_LEVEL] = NULL;
}

int ge_texturedmodel_init(GEModel *model, GETexture *texture, void *indices,
                          void *vertices, GEType index_type,
                          GEType vertex_type, size_t index_num,
                          size_t vertex_num, size_t item_size, int updatable,
                          void *extra) {
    GETexturedModel *texturedmodel = malloc(sizeof(GETexturedModel));
    if(texturedmodel == NULL){
        return GE_E_OUT_OF_MEM;
    }
    if(ge_stdmodel_init(model, indices, vertices, index_type, vertex_type,
                        index_num, vertex_num, item_size, updatable,
                        texturedmodel)){
        free(texturedmodel);
        return GE_E_STDMODEL_INIT;
    }
    if(ge_model_set_callbacks(model, _ge_texturedmodel_before_rendering, NULL,
                              NULL, _ge_textured_after_free, 1)){
        ge_model_free(model);
        free(texturedmodel);
        return GE_E_SET_CALLBACKS;
    }
#if GE_TEXTUREDMODEL_INHERIT_LEVEL+1 >= GE_MODEL_INHERIT_MAX
    Stop compiling right now!
#endif
    model->extra[GE_TEXTUREDMODEL_INHERIT_LEVEL+1] = extra;
    texturedmodel->texture = texture;
    texturedmodel->tex_pos = NULL;
    return GE_E_NONE;
}

int ge_texturedmodel_set_texture(GEModel *model, GEShaderPos *tex_pos,
                                 GEShaderPos *uv_max_pos) {
    GETexturedModel *texturedmodel =
                                  model->extra[GE_TEXTUREDMODEL_INHERIT_LEVEL];
    texturedmodel->tex_pos = tex_pos;
    texturedmodel->uv_max_pos = uv_max_pos;
    return GE_E_NONE;
}

GETexture *ge_texturedmodel_get_texture(GEModel *model) {
    GETexturedModel *texturedmodel =
                                  model->extra[GE_TEXTUREDMODEL_INHERIT_LEVEL];
    return texturedmodel->texture;
}

