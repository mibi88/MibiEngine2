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
#include <time.h>
#include <string.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <EGL/eglplatform.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>

#include <mibiengine2/base/config.h>

#include <mibiengine2/errors.h>

/*
 * Useful links:
 * https://www.saschawillems.de/blog/2015/04/19/using-opengl-es-on-windows-desk
 * tops-via-egl/
 * https://jonmacey.blogspot.com/2012/05/getting-started-with-egl-on-raspberry.
 * html
 * https://www.learnopengles.com/android-lesson-one-getting-started/
 */

#define DEF_CASE(d) case d: return #d;

#define _GE_GLES_WINDOW_BUTTON_NUM 5

#if GE_WINDOW_DEBUG
void _ge_window_debug_callback(GLenum source, GLenum type, GLuint id,
                               GLenum severity, GLsizei length,
                               const GLchar *message,
                               const void *userParam) {
    printf("source: %d, type: %d, id: %d, severity: %d, length: %d, "
           "message: %s\n", source, type, id, severity, length, message);
}
#endif

char *_ge_window_egl_error_str(EGLint error) {
    switch(error){
        DEF_CASE(EGL_SUCCESS)
        DEF_CASE(EGL_NOT_INITIALIZED)
        DEF_CASE(EGL_BAD_ACCESS)
        DEF_CASE(EGL_BAD_ALLOC)
        DEF_CASE(EGL_BAD_ATTRIBUTE)
        DEF_CASE(EGL_BAD_CONTEXT)
        DEF_CASE(EGL_BAD_CONFIG)
        DEF_CASE(EGL_BAD_CURRENT_SURFACE)
        DEF_CASE(EGL_BAD_DISPLAY)
        DEF_CASE(EGL_BAD_SURFACE)
        DEF_CASE(EGL_BAD_MATCH)
        DEF_CASE(EGL_BAD_PARAMETER)
        DEF_CASE(EGL_BAD_NATIVE_PIXMAP)
        DEF_CASE(EGL_BAD_NATIVE_WINDOW)
        DEF_CASE(EGL_CONTEXT_LOST)
        default:
            return "Unknown error";
    }
}

int _ge_gles_window_init(GEWindow *window, char *title) {
    EGLDisplay *display;
    EGLSurface *surface;
    EGLConfig *config;
    EGLContext *context;
#ifndef __EMSCRIPTEN__
    Window *win;
    Atom *wm_delete_window;
    EGLint surface_attr[] = {EGL_NONE};
    Bool repeat = False;
    Bool repeat_set;
#endif
    
    EGLint attr[] = {
        EGL_BUFFER_SIZE, 0,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 0,
        EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER,
        EGL_DEPTH_SIZE, 24,
        EGL_LEVEL, 0,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_SAMPLE_BUFFERS, 0,
        EGL_SAMPLES, 0,
        EGL_STENCIL_SIZE, 0,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_TRANSPARENT_TYPE, EGL_NONE,
        EGL_TRANSPARENT_RED_VALUE, EGL_DONT_CARE,
        EGL_TRANSPARENT_GREEN_VALUE, EGL_DONT_CARE,
        EGL_TRANSPARENT_BLUE_VALUE, EGL_DONT_CARE,
        EGL_CONFIG_CAVEAT, EGL_DONT_CARE,
        EGL_CONFIG_ID, EGL_DONT_CARE,
        EGL_MAX_SWAP_INTERVAL, EGL_DONT_CARE,
        EGL_MIN_SWAP_INTERVAL, EGL_DONT_CARE,
        EGL_NATIVE_RENDERABLE, EGL_DONT_CARE,
        EGL_NATIVE_VISUAL_TYPE, EGL_DONT_CARE,
        EGL_NONE
    };
    EGLint context_attr[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
#if DEBUG_GLES
        EGL_CONTEXT_OPENGL_DEBUG, EGL_TRUE,
#endif
        EGL_NONE
    };
    EGLint min, maj;
    EGLint config_num = 0;
    
    /* Allocate all the values in the structure */
    window->egl.display = malloc(sizeof(EGLDisplay));
    window->egl.surface = malloc(sizeof(EGLSurface));
    window->egl.config = malloc(sizeof(EGLConfig));
    window->egl.context = malloc(sizeof(EGLContext));
#ifndef __EMSCRIPTEN__
    /* Xlib platform dependant stuff */
    window->platform.window = malloc(sizeof(Window));
    window->platform.wm_delete_window = malloc(sizeof(Atom));
    if(window->egl.display == NULL ||
       window->egl.surface == NULL ||
       window->egl.config == NULL ||
       window->egl.context == NULL ||
       window->platform.window == NULL ||
       window->platform.wm_delete_window == NULL){
        free(window->egl.display);
        window->egl.display = NULL;
        free(window->egl.surface);
        window->egl.surface = NULL;
        free(window->egl.config);
        window->egl.config = NULL;
        free(window->egl.context);
        window->egl.context = NULL;
        free(window->platform.window);
        window->platform.window = NULL;
        free(window->platform.wm_delete_window);
        window->platform.wm_delete_window = NULL;
        return GE_E_OUT_OF_MEM;
    }
    win = window->platform.window;
    wm_delete_window = window->platform.wm_delete_window;
#else
    (void)title;
#endif
    display = window->egl.display;
    surface = window->egl.surface;
    config = window->egl.config;
    context = window->egl.context;
    
    /* Create the window and an OpenGL ES context */
#ifndef __EMSCRIPTEN__
    window->platform.display = XOpenDisplay(NULL);
    *win = XCreateSimpleWindow(window->platform.display,
                                  XDefaultRootWindow(window->platform.display),
                                  0, 0, 480, 360, 0, 0, 0xFFFFFF);
    *wm_delete_window = XInternAtom(window->platform.display,
                                    "WM_DELETE_WINDOW", False);
    XSetWMProtocols(window->platform.display, *win, wm_delete_window, 1);
    XSelectInput(window->platform.display, *win,
                 ExposureMask | StructureNotifyMask | KeyPressMask |
                 KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                 PointerMotionMask);
    XStoreName(window->platform.display, *win, title);
    XMapWindow(window->platform.display, *win);
    XFlush(window->platform.display);
    
    repeat_set = XkbSetDetectableAutoRepeat(window->platform.display, True,
                                            &repeat);
    
    if(!repeat || !repeat_set){
        fputs("Key presses may behave unexpectedly because "
              "DetectableAutoRepeat is unsupported!\n", stderr);
    }
#endif
    
    *display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(*display == EGL_NO_DISPLAY){
        fputs("No display found!\n", stderr);
        return GE_E_NO_DISPLAY;
    }
    if(eglInitialize(*display, &maj, &min) == EGL_FALSE){
        fputs("Failed to initialize EGL!\n", stderr);
        return GE_E_EGL_INIT;
    }
    printf("EGL version: %d.%d\n", maj, min);
    /*if(eglGetConfigs(display, NULL, 0, &config_num) == EGL_FALSE){
        fputs("Failed to get config num!\n", stderr);
        return EXIT_FAILURE;
    }
    printf("%d configs available!\n", config_num);
    configs = malloc(config_num*sizeof(EGLConfig));
    if(configs == NULL){
        fputs("Failed to allocate configs array!\n", stderr);
        return EXIT_FAILURE;
    }
    if(eglGetConfigs(display, configs, config_num, &config_num) == EGL_FALSE){
        fputs("Failed to get configs!\n", stderr);
        return EXIT_FAILURE;
    }*/
    
    if(eglBindAPI(EGL_OPENGL_ES_API) == EGL_FALSE){
        fputs("Failed to bind API!\n", stderr);
        return GE_E_API_BIND;
    }
    if(eglChooseConfig(*display, attr, config, 1, &config_num) == EGL_FALSE){
        fputs("Failed to choose config!\n", stderr);
        return GE_E_CHOOSE_CONFIG;
    }
#ifndef __EMSCRIPTEN__
    *surface = eglCreateWindowSurface(*display, *config, *win, surface_attr);
#else
    *surface = eglCreateWindowSurface(*display, *config, 0, NULL);
#endif
    if(eglGetError() != EGL_SUCCESS){
        fputs("Failed to create surface!\n", stderr);
        return GE_E_SURFACE;
    }
    
    *context = eglCreateContext(*display, *config, NULL, context_attr);
    if(eglGetError() != EGL_SUCCESS){
        fputs("Failed to create context!\n", stderr);
        return GE_E_CONTEXT;
    }
    
    if(eglMakeCurrent(*display, *surface, *surface, *context) == EGL_FALSE){
        fputs("Failed to make context current!\n", stderr);
        return GE_E_MAKE_CURRENT;
    }
    
    /* Uncap the framerate */
#ifndef __EMSCRIPTEN__
    eglSwapInterval(*display, 0);
#endif
#if GE_WINDOW_DEBUG
    /* Add a debug callback */
    glDebugMessageCallback(_ge_window_debug_callback, NULL);
#endif
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    
    memset(window->platform.keys_down, 0, GE_K_AMOUNT);
    
    window->draw = NULL;
    window->resize = NULL;
    window->keyevent = NULL;
    window->mouseevent = NULL;
    return GE_E_NONE;
}

int _ge_gles_window_set_data(GEWindow *window, void *data) {
    window->data = data;
    return GE_E_NONE;
}

int _ge_gles_window_cap_framerate(GEWindow *window, int cap) {
#ifndef __EMSCRIPTEN__
    return eglSwapInterval(*(EGLDisplay*)window->platform.display,
                           cap ? 1 : 0) == GL_TRUE;
#else
    (void)window;
    (void)cap;
    /* TODO */
    return GE_E_NONE;
#endif
}

void _ge_gles_window_depth_test(GEWindow *window, int depth_test) {
    (void)window;
    if(depth_test) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);
}

void _ge_gles_window_blending(GEWindow *window, int blend) {
    (void)window;
    if(blend){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }else{
        glDisable(GL_BLEND);
    }
}

unsigned long _ge_gles_window_ms(GEWindow *window) {
    struct timespec time;
    (void)window;
    clock_gettime(CLOCK_REALTIME, &time);
    return time.tv_nsec/(unsigned long)(1e6)+time.tv_sec*1000;
}

int _ge_gles_window_key_pressed(GEWindow *window, GEKey key) {
    if(key >= 0 && key < GE_K_AMOUNT){
        return window->platform.keys_down[key];
    }
    return 0;
}

#ifndef __EMSCRIPTEN__
void _ge_gles_window_mainloop(GEWindow *window) {
    XEvent event;
    KeySym xlib_keys[GE_K_AMOUNT] = {
        XK_VoidSymbol,
        
        XK_Escape,
        XK_F1,
        XK_F2,
        XK_F3,
        XK_F4,
        XK_F5,
        XK_F6,
        XK_F7,
        XK_F8,
        XK_F9,
        XK_F10,
        XK_F11,
        XK_F12,
        XK_Insert,
        XK_Delete,
        XK_Tab,
        XK_Caps_Lock,
        XK_Shift_L,
        XK_Control_L,
        XK_Super_L,
        XK_Alt_L,
        XK_Alt_R,
        XK_Control_R,
        XK_Return,
        XK_BackSpace,
        XK_Up,
        XK_Down,
        XK_Left,
        XK_Right,
        XK_Page_Up,
        XK_Page_Down,
        XK_Home,
        XK_End,
        
        XK_space,
        XK_exclam,
        XK_quotedbl,
        XK_numbersign,
        XK_dollar,
        XK_percent,
        XK_ampersand,
        XK_apostrophe,
        XK_parenleft,
        XK_parenright,
        XK_asterisk,
        XK_plus,
        XK_comma,
        XK_minus,
        XK_period,
        XK_slash,
        XK_0,
        XK_1,
        XK_2,
        XK_3,
        XK_4,
        XK_5,
        XK_6,
        XK_7,
        XK_8,
        XK_9,
        XK_colon,
        XK_semicolon,
        XK_less,
        XK_equal,
        XK_greater,
        XK_question,
        XK_at,
        XK_A,
        XK_B,
        XK_C,
        XK_D,
        XK_E,
        XK_F,
        XK_G,
        XK_H,
        XK_I,
        XK_J,
        XK_K,
        XK_L,
        XK_M,
        XK_N,
        XK_O,
        XK_P,
        XK_Q,
        XK_R,
        XK_S,
        XK_T,
        XK_U,
        XK_V,
        XK_W,
        XK_X,
        XK_Y,
        XK_Z,
        XK_bracketleft,
        XK_backslash,
        XK_bracketright,
        XK_asciicircum,
        XK_underscore,
        XK_quoteleft,
        XK_a,
        XK_b,
        XK_c,
        XK_d,
        XK_e,
        XK_f,
        XK_g,
        XK_h,
        XK_i,
        XK_j,
        XK_k,
        XK_l,
        XK_m,
        XK_n,
        XK_o,
        XK_p,
        XK_q,
        XK_r,
        XK_s,
        XK_t,
        XK_u,
        XK_v,
        XK_w,
        XK_x,
        XK_y,
        XK_z,
        XK_braceleft,
        XK_bar,
        XK_braceright,
        XK_asciitilde,
        XK_nobreakspace
    };
    unsigned int buttons[_GE_GLES_WINDOW_BUTTON_NUM] = {
        Button1,
        Button2,
        Button3,
        Button4,
        Button5
    };
    size_t i;
    KeySym keysym;
    
    while(1){
        if(XPending(window->platform.display)){
            XNextEvent(window->platform.display, &event);
            /* Handle events */
            if(event.type == ClientMessage){
                if((Atom)event.xclient.data.l[0] ==
                   *(Atom*)window->platform.wm_delete_window){
                    break;
                }
            }
            switch(event.type){
                case ConfigureNotify:
                    if(window->resize != NULL){
                        window->resize(window->data, event.xconfigure.width,
                                       event.xconfigure.height);
                    }
                    break;
                case KeyPress:
                    keysym = XLookupKeysym(&event.xkey, 0);
                    for(i=0;i<GE_K_AMOUNT;i++){
                        if(keysym == xlib_keys[i]){
                            if(window->keyevent != NULL &&
                               !window->platform.keys_down[i]){
                                window->keyevent(window->data, i, 0);
                            }
                            window->platform.keys_down[i] = 1;
                            break;
                        }
                    }
                    break;
                case KeyRelease:
                    keysym = XLookupKeysym(&event.xkey, 0);
                    for(i=0;i<GE_K_AMOUNT;i++){
                        if(keysym == xlib_keys[i]){
                            if(window->keyevent != NULL &&
                               window->platform.keys_down[i]){
                                window->keyevent(window->data, i, 1);
                            }
                            window->platform.keys_down[i] = 0;
                            break;
                        }
                    }
                    break;
                case ButtonPress:
                    window->platform.mouse_y = event.xbutton.x;
                    window->platform.mouse_x = event.xbutton.y;
                    if(window->mouseevent != NULL){
                        for(i=0;i<_GE_GLES_WINDOW_BUTTON_NUM;i++){
                            if(event.xbutton.button == buttons[i]){
                                window->mouseevent(window->data,
                                                   window->platform.mouse_y,
                                                   window->platform.mouse_x,
                                                   i, 0);
                            }
                        }
                    }
                    break;
                case ButtonRelease:
                    window->platform.mouse_y = event.xbutton.x;
                    window->platform.mouse_x = event.xbutton.y;
                    if(window->mouseevent != NULL){
                        for(i=0;i<_GE_GLES_WINDOW_BUTTON_NUM;i++){
                            if(event.xbutton.button == buttons[i]){
                                window->mouseevent(window->data,
                                                   window->platform.mouse_y,
                                                   window->platform.mouse_x,
                                                   i, 1);
                            }
                        }
                    }
                    break;
                case MotionNotify:
                    window->platform.mouse_y = event.xmotion.x;
                    window->platform.mouse_x = event.xmotion.y;
                    if(window->mouseevent != NULL){
                        window->mouseevent(window->data,
                                           window->platform.mouse_y,
                                           window->platform.mouse_x,
                                           GE_B_MOVE, 0);
                    }
                    break;
                default:
                    break;
            }
        }else{
            if(window->draw != NULL) window->draw(window->data);
            glFlush();
            if(eglSwapBuffers(*(EGLDisplay*)window->egl.display,
                              *(EGLSurface*)window->egl.surface) == EGL_FALSE){
                fprintf(stderr, "Failed to swap buffers!\n    Error: %s\n",
                        _ge_window_egl_error_str(eglGetError()));
            }
        }
    }
}
#else

#define _Bool char

#include <emscripten.h>
#include <emscripten/html5.h>

bool _ge_gles_window_emresize(int type, const struct EmscriptenUiEvent *event,
                              void *data) {
    GEWindow *window = data;
    (void)type;
    if(window->resize != NULL){
        window->resize(window->data, event->windowInnerWidth,
                       event->windowInnerHeight);
    }
    return 0;
}

bool _ge_gles_window_emkey(int type,
                           const struct EmscriptenKeyboardEvent *event,
                           void *data) {
    GEWindow *window = data;
    char *keys[GE_K_AMOUNT] = {
        "",
        "Escape",
        "F1",
        "F2",
        "F3",
        "F4",
        "F5",
        "F6",
        "F7",
        "F8",
        "F9",
        "F10",
        "F11",
        "F12",
        "Insert",
        "Delete",
        "Tab",
        "CapsLock",
        "Shift",
        "Control",
        "",
        "Alt",
        "AltGraph",
        "Control",
        "Enter",
        "Backspace",
        "ArrowUp",
        "ArrowDown",
        "ArrowLeft",
        "ArrowRight",
        "PageUp",
        "PageDown",
        "Home",
        "End",
        " ",
        "!",
        "\"",
        "#",
        "$",
        "%",
        "&",
        "'",
        "(",
        ")",
        "*",
        "+",
        ",",
        "-",
        ".",
        "/",
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        ":",
        ";",
        "<",
        "=",
        ">",
        "?",
        "@",
        "A",
        "B",
        "C",
        "D",
        "E",
        "F",
        "G",
        "H",
        "I",
        "J",
        "K",
        "L",
        "M",
        "N",
        "O",
        "P",
        "Q",
        "R",
        "S",
        "T",
        "U",
        "V",
        "W",
        "X",
        "Y",
        "Z",
        "[",
        "\\",
        "]",
        "`",
        "_",
        "`",
        "a",
        "b",
        "c",
        "d",
        "e",
        "f",
        "g",
        "h",
        "i",
        "j",
        "k",
        "l",
        "m",
        "n",
        "o",
        "p",
        "q",
        "r",
        "s",
        "t",
        "u",
        "v",
        "w",
        "x",
        "y",
        "z",
        "{",
        "|",
        "}",
        "~",
        "\xA0"
    };
    size_t i;
    /* TODO: Use hashes */
    for(i=0;i<GE_K_AMOUNT;i++){
        if(!strcmp(keys[i], event->key)){
            if(window->keyevent != NULL &&
               window->platform.keys_down[i] !=
               (type != EMSCRIPTEN_EVENT_KEYUP ? 1 : 0)){
                window->keyevent(window->data, i,
                                 type == EMSCRIPTEN_EVENT_KEYUP);
            }
            window->platform.keys_down[i] = (type != EMSCRIPTEN_EVENT_KEYUP ?
                                             1 : 0);
            break;
        }
    }
    return 1;
}

bool _ge_gles_window_emmouseclick(int type,
                                  const struct EmscriptenMouseEvent *event,
                                  void *data) {
    GEWindow *window = data;
    window->platform.mouse_x = event->clientX;
    window->platform.mouse_y = event->clientY;
    if(window->mouseevent != NULL){
        window->mouseevent(window->data, window->platform.mouse_x,
                           window->platform.mouse_y, event->button,
                           type != EMSCRIPTEN_EVENT_MOUSEDOWN);
    }
    return 1;
}

bool _ge_gles_window_emmousemove(int type,
                                  const struct EmscriptenMouseEvent *event,
                                  void *data) {
    GEWindow *window = data;
    window->platform.mouse_x = event->clientX;
    window->platform.mouse_y = event->clientY;
    if(window->mouseevent != NULL){
        window->mouseevent(window->data, window->platform.mouse_x,
                           window->platform.mouse_y, GE_B_MOVE, 0);
    }
    return 1;
}

bool _ge_gles_window_emmousescroll(int type,
                                   const struct EmscriptenWheelEvent *event,
                                   void *data) {
    GEWindow *window = data;
    window->platform.mouse_x = event->mouse.clientX;
    window->platform.mouse_y = event->mouse.clientY;
    if(window->mouseevent != NULL){
        window->mouseevent(window->data, window->platform.mouse_x,
                           window->platform.mouse_y, event->deltaY < 0 ?
                           GE_B_SCROLLUP : GE_B_SCROLLDOWN, 0);
        window->mouseevent(window->data, window->platform.mouse_x,
                           window->platform.mouse_y, event->deltaY < 0 ?
                           GE_B_SCROLLUP : GE_B_SCROLLDOWN, 1);
    }
    return 0;
}

void _ge_gles_window_emloop(void *data) {
    GEWindow *window = data;
    if(window->draw != NULL){
        window->draw(window->data);
        glFlush();
        if(eglSwapBuffers(*(EGLDisplay*)window->egl.display,
                          *(EGLSurface*)window->egl.surface) == EGL_FALSE){
            fprintf(stderr, "Failed to swap buffers!\n    Error: %s\n",
                    _ge_window_egl_error_str(eglGetError()));
        }
    }
}

EM_JS(void, _ge_gles_window_emtrigger_resize, (void), {
    window.dispatchEvent(new Event("resize"));
})

void _ge_gles_window_mainloop(GEWindow *window) {
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, window, 0,
                                   _ge_gles_window_emresize);
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, window, 1,
                                    _ge_gles_window_emkey);
    emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, window, 1,
                                  _ge_gles_window_emkey);
    emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, window,
                                      1, _ge_gles_window_emmouseclick);
    emscripten_set_mouseup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, window, 1,
                                    _ge_gles_window_emmouseclick);
    emscripten_set_mousemove_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, window,
                                      1, _ge_gles_window_emmousemove);
    emscripten_set_wheel_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, window, 1,
                                  _ge_gles_window_emmousescroll);
    _ge_gles_window_emtrigger_resize();
    emscripten_set_main_loop_arg(_ge_gles_window_emloop, window, 0, 1);
}
#endif

void _ge_gles_window_clear(GEWindow *window, float r, float g, float b,
                           float a) {
    (void)window;
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void _ge_gles_window_view(GEWindow *window, int w, int h) {
    (void)window;
    glViewport(0, 0, w, h);
}

void _ge_gles_window_free(GEWindow *window) {
    /* Delete all the EGL and Xlib stuff */
    if(eglMakeCurrent(*(EGLDisplay*)window->egl.display, EGL_NO_SURFACE,
                      EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_FALSE){
        fputs("Failed to make surface and context not current anymore!\n",
              stderr);
    }
    if(eglDestroySurface(*(EGLDisplay*)window->egl.display,
                         *(EGLSurface*)window->egl.surface) == EGL_FALSE){
        fputs("Failed to destroy surface!\n", stderr);
    }
    if(eglDestroyContext(*(EGLDisplay*)window->egl.display,
                         *(EGLContext*)window->egl.context) == EGL_FALSE){
        fputs("Failed to destroy context!\n", stderr);
    }
#ifndef __EMSCRIPTEN__
    XDestroyWindow(window->platform.display,
                   *(Window*)window->platform.window);
    XCloseDisplay(window->platform.display);
    if(eglTerminate(*(EGLDisplay*)window->egl.display) == EGL_FALSE){
        fputs("Failed to terminate EGL!\n", stderr);
    }
#endif
    /* Free all the allocated variables */
    free(window->egl.display);
    window->egl.display = NULL;
    free(window->egl.surface);
    window->egl.surface = NULL;
    free(window->egl.config);
    window->egl.config = NULL;
    free(window->egl.context);
    window->egl.context = NULL;
    free(window->platform.window);
    window->platform.window = NULL;
    free(window->platform.wm_delete_window);
    window->platform.wm_delete_window = NULL;
}

