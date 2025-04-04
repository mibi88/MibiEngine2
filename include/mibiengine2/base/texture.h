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

#ifndef GE_TEXTURE_H
#define GE_TEXTURE_H

#include <mibiengine2/base/image.h>

#include <stddef.h>

#include <mibiengine2/base/shader.h>

#include <mibiengine2/base/mat.h>

typedef struct {
    int width, height;
    int size;
    unsigned char *data;
    unsigned int id;
    GEVec2 uv_max;
    unsigned char flip;
} GETexture;

/* ge_texture_init
 *
 * Load a texture from an image.
 *
 * texture: The texture data.
 * image:   The image to load as a texture.
 * linear:  Use linear filtering instead of nearest neighbour filtering.
 * flip:    Flip the texture (textures are loaded as vertically flipped by
 *          default.
 * Returns GE_E_NONE (0) on success or an error code on failure.
 */
int ge_texture_init(GETexture *texture, GEImage *image, int linear, int flip);

/* ge_texture_update
 *
 * Update the contents of a texture.
 *
 * texture: The texture to update.
 * image:   The image data to load into the texture.
 * Returns GE_E_NONE (0) on success or an error code on failure.
 */
int ge_texture_update(GETexture *texture, GEImage *image);

/* ge_texture_use
 *
 * Use a texture.
 *
 * texture: The texture to use.
 * pos:     The position of the sampler in the shader.
 * n:       The texture unit to use.
 */
void ge_texture_use(GETexture *texture, GEShaderPos *pos, size_t n);

/* ge_texture_free
 *
 * Free a texture.
 *
 * texture: The texture to free.
 */
void ge_texture_free(GETexture *texture);

#endif

