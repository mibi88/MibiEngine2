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
#include <mibiengine2/render2d/font.h>

#include <stdlib.h>

size_t _ge_font_ascii_glyph_pos(void *_font, size_t glyph) {
    (void)_font;
    return glyph-0x20;
}

void _ge_font_ascii_glyph_move_cursor(void *_font, size_t glyph, int *x,
                                      int *y) {
    GEFont *font = _font;
    if(glyph >= 96) return;
    switch(glyph){
        case '\n':
            *x = 0;
            *y += font->line_height+font->line_spacing;
            break;
        default:
            *x += font->glyphs[glyph].x2-font->glyphs[glyph].x1+
                  font->char_spacing;
    }
}

int ge_font_init(GEFont *font, GEImage *image, int variable_width,
                 int min_char_width, int padding, float char_spacing,
                 float line_spacing, GECharset charset) {
    size_t i;
    size_t charset_glyph_num[GE_F_AMOUNT] = {
        96
    };
    size_t (*glyph_pos[GE_F_AMOUNT])(void *font, size_t glyph) = {
        _ge_font_ascii_glyph_pos
    };
    void (*glyph_move_cursor[GE_F_AMOUNT])(void *_font, size_t glyph, int *x,
                                           int *y) = {
        _ge_font_ascii_glyph_move_cursor
    };
    int glyph_w, glyph_h;
    int x, y;
    (void)min_char_width;
    
    font->charset = charset;
    font->char_spacing = char_spacing;
    font->line_spacing = line_spacing;
    
    font->glyph_pos = glyph_pos[charset];
    font->glyph_move_cursor = glyph_move_cursor[charset];
    
    font->glyphs = malloc(charset_glyph_num[charset]*sizeof(GEGlyph));
    if(font->glyphs == NULL){
        return GE_E_OUT_OF_MEM;
    }
    
    if(variable_width){
        /* TODO: Support variable width fonts */
    }else{
        glyph_w = image->width/16;
        glyph_h = image->height/6;
        font->line_height = glyph_h;
        for(i=0;i<charset_glyph_num[charset];i++){
            x = i%16;
            y = i/16;
            font->glyphs[i].x1 = -padding;
            font->glyphs[i].y1 = -padding;
            font->glyphs[i].x2 = glyph_w+padding;
            font->glyphs[i].y2 = glyph_h+padding;
            
            font->glyphs[i].u2 = x*(glyph_w+2*padding)/(float)image->width;
            font->glyphs[i].v2 = y*(glyph_h+2*padding)/(float)image->height;
            font->glyphs[i].u1 = font->glyphs[i].u2+(glyph_w+padding)/
                                 (float)image->width;
            font->glyphs[i].v1 = font->glyphs[i].v2+(glyph_h+padding)/
                                 (float)image->height;
        }
    }
    
    return GE_E_NONE;
}

void ge_font_free(GEFont *font) {
    free(font->glyphs);
    font->glyphs = NULL;
}

