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

#ifndef GE_TILEMAP_H
#define GE_TILEMAP_H

#include <mibiengine2/base/texturedmodel.h>
#include <mibiengine2/base/texture.h>
#include <mibiengine2/renderer/stdshader.h>
#include <stddef.h>

typedef struct {
    GEModel model;
    GEImage *tileset;
    unsigned short int *tiles;
    int tiles_w, tiles_h;
    int w, h;
    float *vertices;
    float *uv_coords;
    unsigned short int *indices;
} GETilemap;

int ge_tilemap_init(GETilemap *tilemap, GEImage *tileset, GETexture *texture,
                    GEStdShader *shader, unsigned short int *tiles, int w,
                    int h, int tiles_w, int tiles_h, int size);

int ge_tilemap_update(GETilemap *tilemap, unsigned short int *tiles, int w,
                      int h, int size);

#define GE_TILEMAP_GET_MODEL(tilemap) ((tilemap)->model)

void ge_tilemap_free(GETilemap *tilemap);

#endif

