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

#include <mibiengine2/renderer/renderable.h>

int ge_renderable_init(GERenderable *renderable, void *data,
                       void (*render)(void *data, GEMat4 *mat,
                                      GEMat3 *normal_mat),
                       void (*render_multiple)(void *data, GEMat4 *mats,
                                               GEMat3 *normal_mat,
                                               size_t count),
                       void (*free)(void *data)) {
    renderable->data = data;
    renderable->calls.render = render;
    renderable->calls.render_multiple = render_multiple;
    renderable->calls.free = free;
    return 0;
}

void ge_renderable_render(GERenderable *renderable, GEMat4 *mat,
                          GEMat3 *normal_mat) {
    if(renderable->calls.render){
        renderable->calls.render(renderable->data, mat, normal_mat);
    }
}

void ge_renderable_render_multiple(GERenderable *renderable, GEMat4 *mat,
                                   GEMat3 *normal_mat, size_t count) {
    if(renderable->calls.render_multiple){
        renderable->calls.render_multiple(renderable->data, mat, normal_mat,
                                          count);
    }
}

void ge_renderable_free(GERenderable *renderable) {
    if(renderable->calls.free) renderable->calls.free(renderable->data);
}

