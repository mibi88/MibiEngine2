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

#ifndef GE_FRAMEBUFFER_H
#define GE_FRAMEBUFFER_H

#include <color.h>
#include <stddef.h>

#include <stdmodel.h>

typedef enum {
    GE_TEX_COLOR,
    GE_TEX_DEPTH,
    GE_TEX_STENCIL,
    GE_TEX_AMOUNT
} GETexType;

/* GL_MAX_COLOR_ATTACHMENTS is 1 with OpenGL ES 2 */
#define GE_FRAMEBUFFER_COLOR_TEX_MAX 1
#define GE_FRAMEBUFFER_DEPTH_TEX_MAX 1
#define GE_FRAMEBUFFER_STENCIL_TEX_MAX 1
#define GE_FRAMEBUFFER_TEX_MAX (GE_FRAMEBUFFER_COLOR_TEX_MAX+ \
                                GE_FRAMEBUFFER_DEPTH_TEX_MAX+ \
                                GE_FRAMEBUFFER_STENCIL_TEX_MAX)

typedef struct {
    int width, height;
    int size;
    unsigned int fbo;
    unsigned int tex[GE_FRAMEBUFFER_TEX_MAX];
    /* TODO: Clean this up */
    int tex_pos[GE_FRAMEBUFFER_TEX_MAX];
    int tex_format[GE_FRAMEBUFFER_TEX_MAX];
    int tex_internal[GE_FRAMEBUFFER_TEX_MAX];
    int tex_type[GE_FRAMEBUFFER_TEX_MAX];
    int tex_attachment[GE_FRAMEBUFFER_TEX_MAX];
    size_t tex_num;
    GEModel model;
    GEVec2 tex_size;
    GEShaderPos *size_pos;
} GEFramebuffer;

/* TODO: Test all of this... */
int ge_frambuffer_init(GEFramebuffer *framebuffer, int w, int h,
                       size_t tex_count, GEColor *formats, GETexType *type,
                       char *linear);
int ge_framebuffer_resize(GEFramebuffer *framebuffer, int w, int h);
int ge_framebuffer_attr(GEFramebuffer *framebuffer, GEShader *shader,
                        char **attr_names, char **tex_names,
                        GEShaderPos *size_pos);
void ge_framebuffer_render(GEFramebuffer *framebuffer);
void ge_framebuffer_use(GEFramebuffer *framebuffer);
void ge_framebuffer_default(void);
void ge_framebuffer_free(GEFramebuffer *framebuffer);

#endif

