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

#ifndef GE_WINDOW_H
#define GE_WINDOW_H

#include <mibiengine2/base/keys.h>

#define GE_WINDOW_DEBUG 0

typedef enum {
    GE_B_LEFT,
    GE_B_MIDDLE,
    GE_B_RIGHT,
    GE_B_SCROLLUP,
    GE_B_SCROLLDOWN,
    GE_B_MOVE
} GEWindowMouseEvent;

typedef struct {
    /* Not sure if it's a good idea to use a whole bunch of void pointers like
     * this */
    /* Platform independant EGL stuff */
    struct {
        void *display;
        void *surface;
        void *config;
        void *context;
    } egl;
    /* Platform dependant stuff */
    struct {
        void *display;
        void *window;
        void *wm_delete_window;
        char keys_down[GE_K_AMOUNT];
        int mouse_x, mouse_y;
    } platform;
    /* Callbacks to handle various events */
    void (*draw)(void *data);
    void (*resize)(void *data, int w, int h);
    void (*keyevent)(void *data, int key, int released);
    void (*mouseevent)(void *data, int x, int y, GEWindowMouseEvent event,
                       int released);
    /* Data to pass to the callbacks when they are called */
    void *data;
} GEWindow;

/* ge_window_init
 *
 * Create a window with a rendering context for the right backend.
 * Currently only a single window should be opened at once.
 *
 * window: The window struct containing all the data related to this window.
 * title:  The title of the window.
 * Returns 0 on success or an error code on failure.
 */
int ge_window_init(GEWindow *window, char *title);

/* ge_window_set_callbacks
 *
 * Set the window callbacks. They are called when the window is drawn or when
 * it is resized, for example.
 *
 * window:     The window struct.
 * draw:       The callback called when the window's content should be drawn.
 * resize:     The callback called when the window is resized.
 * keyevent:   The callback called when a key is pressed or released.
 * mouseevent: The callback that gets called when the mouse is moving, a button
 *             is clicked or when scrolling (see GEWindowMouseEvent).
 * Returns 0 on success or an error code on failure.
 */
int ge_window_set_callbacks(GEWindow *window, void draw(void *data),
                            void resize(void *data, int w, int h),
                            void keyevent(void *data, int key, int released),
                            void mouseevent(void *data, int x, int y,
                                            GEWindowMouseEvent event,
                                            int released));

/* ge_window_set_data
 *
 * Set the data passed to callbacks.
 *
 * window: The window struct.
 * data:   The data to pass to callbacks.
 * Returns 0 on success or an error code on failure.
 */
int ge_window_set_data(GEWindow *window, void *data);

/* ge_window_cap_framerate
 *
 * Set if the framerate should be capped, or if the rendering should be done as
 * fast as possible.
 *
 * window: The window struct.
 * cap:    0 if the framerate should be capped, else it will be capped.
 * Returns 0 on success or an error code on failure.
 */
int ge_window_cap_framerate(GEWindow *window, int cap);

/* ge_window_depth_test
 *
 * Enable or disable depth test.
 *
 * window:     The window struct.
 * depth_test: 0 if depth test should be disabled, a non zero value if it
 *             should be enabled.
 */
void ge_window_depth_test(GEWindow *window, int depth_test);

/* ge_window_blending
 *
 * Enable or disable blending.
 *
 * window: The window struct.
 * blend:  0 if blending should be disabled, a non zero value if it should be
 *         enabled.
 */
void ge_window_blending(GEWindow *window, int blend);

/* ge_window_ms
 *
 * Get the elapsed miliseconds.
 *
 * window: The window struct.
 * Returns the number of elapsed miliseconds.
 */
unsigned long ge_window_ms(GEWindow *window);

/* ge_window_key_pressed
 *
 * Check if a key is currently pressed.
 *
 * window: The window struct.
 * key:    The key to check if it is pressed.
 * Returns 0 if the key isn't pressed or a non-zero value if it is pressed.
 */
int ge_window_key_pressed(GEWindow *window, GEKey key);

/* ge_window_mainloop
 *
 * Run the window mainloop.
 * This blocks the program until the window is closed. Callbacks are called for
 * rendering or when the window got resized.
 *
 * window: The window to run the mainloop of.
 */
void ge_window_mainloop(GEWindow *window);

/* ge_window_clear
 *
 * Clear the window contents.
 *
 * window: The window to clear.
 * r:      The red intensity (between 0 and 1) of the clear color.
 * g:      The green intensity (between 0 and 1) of the clear color.
 * b:      The blue intensity (between 0 and 1) of the clear color.
 */
void ge_window_clear(GEWindow *window, float r, float g, float b, float a);

/* ge_window_view
 *
 * Resize the window viewport.
 *
 * window: The window to resize the viewport of.
 * w:      The new width of the window.
 * h:      The new height of the window.
 */
void ge_window_view(GEWindow *window, int w, int h);

/* ge_window_free
 *
 * Destroy a window and free the context associed with it.
 *
 * window: The window to destroy.
 */
void ge_window_free(GEWindow *window);

#endif

