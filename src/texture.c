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

#include <texture.h>

#include <GLES2/gl2.h>

#include <stdlib.h>
#include <string.h>

int _ge_texture_get_size(int size) {
    /* TODO: Provide such a function in a different file, to share it between
     * multiple files */
    /* Find the closest power of two */
    int c;
    for(c=1;size>>=1;c++);
    return 1<<c;
}

int ge_texture_init(GETexture *texture, GEImage *image, int linear, int flip) {
    size_t x, y;
    size_t tex_y;
    int bytes;
    unsigned char color[4];
    /* Make a copy of the texture in RGBA color format as a square texture */
    texture->size = _ge_texture_get_size(image->width > image->height ?
                                         image->width : image->height);
    texture->width = image->width;
    texture->height = image->height;
    texture->uv_max.x = image->width/(float)texture->size;
    texture->uv_max.y = image->height/(float)texture->size;
    texture->data = malloc(texture->size*texture->size*4);
    if(texture->data == NULL){
        return 1;
    }
    bytes = image->row_bytes/image->width;
    memset(texture->data, 0, texture->size*texture->size*4);
    for(y=0;y<image->height;y++){
        for(x=0;x<image->width;x++){
            memcpy(color, image->data+(y*image->width+x)*bytes,
                   bytes <= 4 ? bytes : 4);
            if(4-bytes > 0) memset(color+bytes, 255, 4-bytes);
            if(flip) tex_y = (texture->size-y-1);
            else tex_y = y;
            memcpy(texture->data+(tex_y*texture->size+x)*4,
                   color, 4);
        }
    }
    /* Upload the texture to the GPU */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    linear ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->size, texture->size, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
    return 0;
}

void ge_texture_use(GETexture *texture, GEShaderPos *pos, size_t n) {
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform1i(pos->pos, n);
}

void ge_texture_free(GETexture *texture) {
    free(texture->data);
    texture->data = NULL;
    glDeleteTextures(1, &texture->id);
}

