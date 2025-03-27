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

enum {
    /* Base */
    GE_E_SUCCESS,
    GE_E_OUT_OF_MEM,
    GE_E_STDMODEL_INIT,
    GE_E_STDMODEL_ADD,
    GE_E_FRAMEBUFFER_INCOMPLETE,
    GE_E_SET_ATTR,
    GE_E_NO_DISPLAY,
    GE_E_EGL_INIT,
    GE_E_API_BIND,
    GE_E_CHOOSE_CONFIG,
    GE_E_SURFACE,
    GE_E_CONTEXT,
    GE_E_MAKE_CURRENT,
    GE_E_NOT_PNG,
    GE_E_FILE,
    GE_E_MODELARRAY_INIT,
    GE_E_MODEL_INIT,
    GE_E_SET_CALLBACKS,
    GE_E_OBJ_LOADING,
    GE_E_TEXTUREDMODEL_INIT,
    GE_E_SET_TEXTURE,
    /* Renderer */
    
    /* Other */
    GE_E_UNKNOWN,
    GE_E_AMOUNT
};

