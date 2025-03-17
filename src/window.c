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

#include <window.h>

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <EGL/eglplatform.h>
#include <X11/Xlib.h>

#include <config.h>

#define DEF_CASE(d) case d: return #d;

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

int ge_window_create(GEWindow *window, char *title) {
    EGLDisplay *display;
    EGLSurface *surface;
    EGLConfig *config;
    EGLContext *context;
    Window *win;
    Atom *wm_delete_window;
    
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
    EGLint surface_attr[] = {EGL_NONE};
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
        return 1;
    }
    display = window->egl.display;
    surface = window->egl.surface;
    config = window->egl.config;
    context = window->egl.context;
    win = window->platform.window;
    wm_delete_window = window->platform.wm_delete_window;
    
    /* Create the window and an OpenGL ES context */
    window->platform.display = XOpenDisplay(NULL);
    *win = XCreateSimpleWindow(window->platform.display,
                                  XDefaultRootWindow(window->platform.display),
                                  0, 0, 480, 360, 0, 0, 0xFFFFFF);
    *wm_delete_window = XInternAtom(window->platform.display,
                                    "WM_DELETE_WINDOW", False);
    XSetWMProtocols(window->platform.display, *win, wm_delete_window, 1);
    XSelectInput(window->platform.display, *win,
                 ExposureMask | StructureNotifyMask);
    XStoreName(window->platform.display, *win, title);
    XMapWindow(window->platform.display, *win);
    XFlush(window->platform.display);
    
    *display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(*display == EGL_NO_DISPLAY){
        fputs("No display found!\n", stderr);
        return 2;
    }
    if(eglInitialize(*display, &maj, &min) == EGL_FALSE){
        fputs("Failed to initialize EGL!\n", stderr);
        return 3;
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
        return 4;
    }
    if(eglChooseConfig(*display, attr, config, 1, &config_num) == EGL_FALSE){
        fputs("Failed to choose config!\n", stderr);
        return 5;
    }
    *surface = eglCreateWindowSurface(*display, *config, *win, surface_attr);
    if(eglGetError() != EGL_SUCCESS){
        fputs("Failed to create surface!\n", stderr);
        return 6;
    }
    
    *context = eglCreateContext(*display, *config, NULL, context_attr);
    if(eglGetError() != EGL_SUCCESS){
        fputs("Failed to create context!\n", stderr);
        return 7;
    }
    
    if(eglMakeCurrent(*display, *surface, *surface, *context) == EGL_FALSE){
        fputs("Failed to make context current!\n", stderr);
        return 8;
    }
    
    /* Uncap the framerate */
    eglSwapInterval(*display, 0);
#if GE_WINDOW_DEBUG
    /* Add a debug callback */
    glDebugMessageCallback(_ge_window_debug_callback, NULL);
#endif
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    return 0;
}

void ge_window_mainloop(GEWindow *window) {
    XEvent event;
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
                default:
                    break;
            }
        }else{
            if(window->draw != NULL) window->draw(window->data);
            if(eglSwapBuffers(*(EGLDisplay*)window->egl.display,
                              *(EGLSurface*)window->egl.surface) == EGL_FALSE){
                fprintf(stderr, "Failed to swap buffers!\n    Error: %s\n",
                        _ge_window_egl_error_str(eglGetError()));
            }
        }
    }
}

void ge_window_clear(GEWindow *window, float r, float g, float b, float a) {
    (void)window;
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ge_window_view(GEWindow *window, int w, int h) {
    (void)window;
    glViewport(0, 0, w, h);
}

void ge_window_free(GEWindow *window) {
    /* Delete all the EGL and Xlib stuff */
    /* TODO */
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

