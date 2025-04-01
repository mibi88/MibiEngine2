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

#include <mibiengine2/base/window.h>

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <backendlist.h>

#include <mibiengine2/base/config.h>
#include <mibiengine2/errors.h>

int ge_window_init(GEWindow *window, char *title) {
    return GE_BACKENDLIST_GET(window_init)(window, title);
}

int ge_window_set_callbacks(GEWindow *window, void draw(void *data),
                            void resize(void *data, int w, int h),
                            void keyevent(void *data, int key, int released)) {
    window->draw = draw;
    window->resize = resize;
    window->keyevent = keyevent;
    return GE_E_NONE;
}

int ge_window_set_data(GEWindow *window, void *data) {
    return GE_BACKENDLIST_GET(window_set_data)(window, data);
}

int ge_window_cap_framerate(GEWindow *window, int cap) {
    return GE_BACKENDLIST_GET(window_cap_framerate)(window, cap);
}

void ge_window_depth_test(GEWindow *window, int depth_test) {
    GE_BACKENDLIST_GET(window_depth_test)(window, depth_test);
}

unsigned long ge_window_ms(GEWindow *window) {
    return GE_BACKENDLIST_GET(window_ms)(window);
}

int ge_window_key_pressed(GEWindow *window, GEKey key) {
    return GE_BACKENDLIST_GET(window_key_pressed)(window, key);
}

void ge_window_mainloop(GEWindow *window) {
    GE_BACKENDLIST_GET(window_mainloop)(window);
}

void ge_window_clear(GEWindow *window, float r, float g, float b, float a) {
    GE_BACKENDLIST_GET(window_clear)(window, r, g, b, a);
}

void ge_window_view(GEWindow *window, int w, int h) {
    GE_BACKENDLIST_GET(window_view)(window, w, h);
}

void ge_window_free(GEWindow *window) {
    GE_BACKENDLIST_GET(window_free)(window);
}

