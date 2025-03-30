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

#ifndef GE_TEXTUREDMODEL_H
#define GE_TEXTUREDMODEL_H

#define GE_TEXTUREDMODEL_INHERIT_LEVEL (GE_STDMODEL_INHERIT_LEVEL+1)

#include <mibiengine2/base/stdmodel.h>
#include <mibiengine2/base/texture.h>

typedef struct {
    GETexture *texture;
    GEShaderPos *tex_pos;
    GEShaderPos *uv_max_pos;
} GETexturedModel;

/* ge_texturedmodel_init
 *
 * Create a textured model. It is the same as a standard model (see
 * stdmodel.h), it has just a texture attached to it.
 *
 * model:       The model to initialize.
 * texture:     The texture to use with this model.
 * indices:     The model indices (see model.h).
 * vertices:    The position of the vertices of the model.
 * index_type:  The type of the index data (see type.h).
 * vertex_type: The type of the vertex data (see type.h).
 * index_num:   The number of indices.
 * vertex_num:  The number of vertex positions.
 * item_size:   The size of a single vertex position.
 * extra:       Extra data (see model.h and base.h).
 * Returns 0 on success or an error code on failure.
 */
int ge_texturedmodel_init(GEModel *model, GETexture *texture, void *indices,
                          void *vertices, GEType index_type,
                          GEType vertex_type, size_t index_num,
                          size_t vertex_num, size_t item_size, void *extra);

/* ge_texturedmodel_set_texture
 *
 * Set the position of the texture sampler in the shader used with this model.
 *
 * model:      The model to set the texture position to.
 * tex_pos:    The texture sampler position.
 * uv_max_pos: The position of the UV max. uniform. Textures are square and
 *             power of two sized. When the image they contain isn't square,
 *             (or power of two sized) the UV coordinates need to be adapted to
 *             use the correct part of the texture.
 * Returns 0 on success or an error code on failure.
 */
int ge_texturedmodel_set_texture(GEModel *model, GEShaderPos *tex_pos,
                                 GEShaderPos *uv_max_pos);

/* ge_texturedmodel_get_texture
 *
 * Get the texture used with a textured model.
 *
 * model: The model to get the texture of.
 * Returns a pointer to the texture data.
 */
GETexture *ge_texturedmodel_get_texture(GEModel *model);

#endif

