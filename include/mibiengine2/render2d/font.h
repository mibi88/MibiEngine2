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

#ifndef GE_FONT_H
#define GE_FONT_H

#include <mibiengine2/base/image.h>
#include <stddef.h>

typedef enum {
    GE_F_ASCII,
    GE_F_AMOUNT
} GECharset;

typedef struct {
    int x1, y1;
    int x2, y2;
    float u1, v1;
    float u2, v2;
} GEGlyph;

typedef struct {
    GEImage *image;
    GEGlyph *glyphs;
    size_t glyph_num;
    unsigned char charset;
    int char_spacing;
    int line_spacing;
    int line_height;
    
    size_t (*glyph_pos)(void *font, size_t glyph);
    void (*glyph_move_cursor)(void *_font, size_t glyph, int *x, int *y);
} GEFont;

int ge_font_init(GEFont *font, GEImage *image, int variable_width,
                 int min_char_width, int padding, float char_spacing,
                 float line_spacing, GECharset charset);

void ge_font_free(GEFont *font);

#endif

