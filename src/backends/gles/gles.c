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

#include <gles.h>

GEBackend _ge_gles_backend = {
    _ge_gles_framebuffer_init,
    _ge_gles_framebuffer_resize,
    _ge_gles_framebuffer_attr,
    _ge_gles_framebuffer_render,
    _ge_gles_framebuffer_use,
    _ge_gles_framebuffer_default,
    _ge_gles_framebuffer_free,
    
    _ge_gles_model_init,
    _ge_gles_model_set_attr,
    _ge_gles_model_render,
    _ge_gles_model_render_multiple,
    _ge_gles_model_attr_init,
    _ge_gles_model_free,
    
    _ge_gles_modelarray_init,
    _ge_gles_modelarray_update,
    _ge_gles_modelarray_enable,
    _ge_gles_modelarray_disable,
    _ge_gles_modelarray_free,
    
    _ge_gles_shader_init,
    _ge_gles_shader_use,
    _ge_gles_shader_get_pos,
    _ge_gles_shader_load_mat4,
    _ge_gles_shader_load_mat3,
    _ge_gles_shader_load_vec4,
    _ge_gles_shader_load_vec3,
    _ge_gles_shader_load_vec2,
    _ge_gles_shader_free,
    
    _ge_gles_texture_init,
    _ge_gles_texture_update,
    _ge_gles_texture_use,
    _ge_gles_texture_free,
    
    _ge_gles_window_init,
    _ge_gles_window_set_data,
    _ge_gles_window_cap_framerate,
    _ge_gles_window_depth_test,
    _ge_gles_window_ms,
    _ge_gles_window_key_pressed,
    _ge_gles_window_mainloop,
    _ge_gles_window_clear,
    _ge_gles_window_view,
    _ge_gles_window_free
};
