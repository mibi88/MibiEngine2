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

#include <mibiengine2/base/framebuffer.h>

#include <mibiengine2/base/utils.h>
#include <mibiengine2/errors.h>

#include <GLES2/gl2.h>

int _ge_gles_framebuffer_init(GEFramebuffer *framebuffer, int w, int h,
                              size_t tex_count, GEColor *formats,
                              GETexType *type, char *linear) {
    /* Model data */
    float vertices[4*2] = {
        -1,  1,
        -1, -1,
         1, -1,
         1,  1
    };
    unsigned short int indices[6] = {
        0, 1, 3,
        3, 1, 2
    };
    float uv_coords[4*2] = {
        0, 0,
        0, 1,
        1, 1,
        1, 0
    };
    /* Framebuffer related variables */
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
    /* Initialize the model used to render the framebuffer */
    if(ge_stdmodel_init(&framebuffer->model, indices, vertices, GE_T_USHORT,
                        GE_T_FLOAT, 6, 4*2, 2, 0, NULL)){
        return GE_E_STDMODEL_INIT;
    }
    if(ge_stdmodel_add_uv_coords(&framebuffer->model, uv_coords, GE_T_FLOAT,
                                 4*2, 2)){
        ge_model_free(&framebuffer->model);
        return GE_E_STDMODEL_ADD;
    }
    
    /* Create the framebuffer */
    framebuffer->tex_num = tex_count;
    framebuffer->width = w;
    framebuffer->height = h;
    framebuffer->size = ge_utils_power_of_two(w > h ? w : h);
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
                
                framebuffer->tex_internal[tex_pos] =
                                              gl_colors_internal[formats[i]];
                framebuffer->tex_format[tex_pos] = gl_colors[formats[i]];
                framebuffer->tex_type[tex_pos] = gl_color_type[formats[i]];
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                linear[i] ? GL_LINEAR : GL_NEAREST);
                
                /* Attach the texture to the framebuffer */
                glFramebufferTexture2D(GL_FRAMEBUFFER,
                                       attachments[color_attachments],
                                       GL_TEXTURE_2D,
                                       framebuffer->tex[tex_pos], 0);
                framebuffer->tex_attachment[tex_pos] =
                                              attachments[color_attachments];
                color_attachments++;
                tex_pos++;
                break;
            case GE_TEX_DEPTH:
                if(depth_attachments >= GE_FRAMEBUFFER_DEPTH_TEX_MAX) break;
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                             framebuffer->size, framebuffer->size, 0,
                             GL_DEPTH_COMPONENT, GL_UNSIGNED_INT,
                             NULL);
                
                framebuffer->tex_internal[tex_pos] = GL_DEPTH_COMPONENT;
                framebuffer->tex_format[tex_pos] = GL_DEPTH_COMPONENT;
                framebuffer->tex_type[tex_pos] = GL_UNSIGNED_INT;
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                linear[i] ? GL_LINEAR : GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                GL_NEAREST);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                       GL_TEXTURE_2D,
                                       framebuffer->tex[tex_pos], 0);
                framebuffer->tex_attachment[tex_pos] = GL_DEPTH_ATTACHMENT;
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
    framebuffer->tex_num = tex_pos;
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        glDeleteFramebuffers(1, &framebuffer->fbo);
        glDeleteTextures(1, framebuffer->tex);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        return GE_E_FRAMEBUFFER_INCOMPLETE;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    framebuffer->tex_size.x = w/(float)framebuffer->size;
    framebuffer->tex_size.y = h/(float)framebuffer->size;
    return GE_E_NONE;
}

int _ge_gles_framebuffer_resize(GEFramebuffer *framebuffer, int w, int h) {
    /* TODO: Clean this up */
    size_t i;
    framebuffer->width = w;
    framebuffer->height = h;
    framebuffer->size = ge_utils_power_of_two(w > h ? w : h);
    
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);
    for(i=0;i<framebuffer->tex_num;i++){
        glBindTexture(GL_TEXTURE_2D, framebuffer->tex[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, framebuffer->tex_internal[i],
                     framebuffer->size, framebuffer->size, 0,
                     framebuffer->tex_format[i], framebuffer->tex_type[i],
                     NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, framebuffer->tex_attachment[i],
                               GL_TEXTURE_2D, framebuffer->tex[i], 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    framebuffer->tex_size.x = w/(float)framebuffer->size;
    framebuffer->tex_size.y = h/(float)framebuffer->size;
    return GE_E_NONE;
}

int _ge_gles_framebuffer_attr(GEFramebuffer *framebuffer, GEShader *shader,
                              char **attr_names, char **tex_names,
                              GEShaderPos *size_pos) {
    size_t i;
    for(i=0;i<framebuffer->tex_num;i++){
        framebuffer->tex_pos[i] = glGetUniformLocation(shader->shader_program,
                                                       tex_names[i]);
    }
    if(ge_stdmodel_shader_attr(&framebuffer->model, shader, attr_names)){
        return GE_E_SET_ATTR;
    }
    framebuffer->size_pos = size_pos;
    return GE_E_NONE;
}

void _ge_gles_framebuffer_render(GEFramebuffer *framebuffer) {
    size_t i;
    int textures[GE_FRAMEBUFFER_TEX_MAX] = {
        GL_TEXTURE0,
        GL_TEXTURE1,
        GL_TEXTURE2
    };
    ge_shader_load_vec2(framebuffer->size_pos, &framebuffer->tex_size);
    for(i=0;i<framebuffer->tex_num;i++){
        glActiveTexture(textures[i]);
        glBindTexture(GL_TEXTURE_2D, framebuffer->tex[i]);
        glUniform1i(framebuffer->tex_pos[i], i);
    }
    glDisable(GL_DEPTH_TEST);
    ge_model_render(&framebuffer->model);
    glEnable(GL_DEPTH_TEST);
    for(i=0;i<framebuffer->tex_num;i++){
        glActiveTexture(textures[i]);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glActiveTexture(GL_TEXTURE0);
}

void _ge_gles_framebuffer_use(GEFramebuffer *framebuffer) {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->fbo);
}

void _ge_gles_framebuffer_default(void) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void _ge_gles_framebuffer_free(GEFramebuffer *framebuffer) {
    glDeleteFramebuffers(1, &framebuffer->fbo);
    glDeleteTextures(framebuffer->tex_num, framebuffer->tex);
    ge_model_free(&framebuffer->model);
}

