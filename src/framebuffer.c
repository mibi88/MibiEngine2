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

#include <framebuffer.h>

#include <GLES2/gl2.h>

int _ge_framebuffer_get_size(int size) {
    /* TODO: Provide such a function in a different file, to share it between
     * multiple files */
    /* Find the closest power of two */
    int c;
    for(c=1;size>>=1;c++);
    return 1<<c;
}

int ge_frambuffer_init(GEFramebuffer *framebuffer, int w, int h,
                       size_t tex_count, GEColor *formats, GETexType *type,
                       char *linear) {
    size_t i;
    int attachments[GE_FRAMEBUFFER_COLOR_TEX_MAX] = {
        GL_COLOR_ATTACHMENT0
    };
    int gl_colors[GE_C_AMOUNT] = {
        GL_RGB,
        GL_RGBA
    };
    int gl_colors_internal[GE_C_AMOUNT] = {
        GL_RGB,
        GL_RGBA
    };
    int gl_color_type[GE_C_AMOUNT] = {
        GL_UNSIGNED_BYTE,
        GL_UNSIGNED_BYTE
    };
    size_t color_attachments = 0;
    size_t depth_attachments = 0;
    size_t stencil_attachments = 0;
    size_t tex_pos = 0;
    framebuffer->tex_num = tex_count;
    framebuffer->width = w;
    framebuffer->height = h;
    framebuffer->size = _ge_framebuffer_get_size(w > h ? w : h);
    /* XXX: Is it better that I create a square power of two texture for the
     * FBO? */
    glGenFramebuffers(1, &framebuffer->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);
    
    for(i=0;i<tex_count;i++){
        /* Create the texture which will hold the data */
        glGenTextures(1, framebuffer->tex+tex_pos);
        glBindTexture(GL_TEXTURE_2D, framebuffer->tex[tex_pos]);
        
        switch(type[i]){
            case GE_TEX_COLOR:
                if(color_attachments >= GE_FRAMEBUFFER_COLOR_TEX_MAX) break;
                glTexImage2D(GL_TEXTURE_2D, 0, gl_colors_internal[formats[i]],
                             framebuffer->size, framebuffer->size, 0,
                             gl_colors[formats[i]], gl_color_type[formats[i]],
                             NULL);
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                linear[i] ? GL_LINEAR : GL_NEAREST);
                
                /* Attach the texture to the framebuffer */
                glFramebufferTexture2D(GL_FRAMEBUFFER,
                                       attachments[color_attachments],
                                       GL_TEXTURE_2D, framebuffer->tex[i], 0);
                color_attachments++;
                tex_pos++;
                break;
            case GE_TEX_DEPTH:
                if(depth_attachments >= GE_FRAMEBUFFER_DEPTH_TEX_MAX) break;
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                             framebuffer->size, framebuffer->size, 0,
                             GL_DEPTH_COMPONENT, GL_UNSIGNED_INT,
                             NULL);
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                linear[i] ? GL_LINEAR : GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                GL_NEAREST);
                depth_attachments++;
                tex_pos++;
                break;
            case GE_TEX_STENCIL:
                if(stencil_attachments >= GE_FRAMEBUFFER_STENCIL_TEX_MAX){
                    break;
                }
                /* TODO: Implement this */
                stencil_attachments++;
                tex_pos++;
                break;
            default:
                break;
        }
    }
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        glDeleteFramebuffers(1, &framebuffer->fbo);
        glDeleteTextures(1, framebuffer->tex);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        return 1;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    return 0;
}

void ge_framebuffer_free(GEFramebuffer *framebuffer) {
    glDeleteFramebuffers(1, &framebuffer->fbo);
    glDeleteTextures(framebuffer->tex_num, framebuffer->tex);
}

