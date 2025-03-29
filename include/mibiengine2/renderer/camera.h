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

#ifndef GE_CAMERA_H
#define GE_CAMERA_H

#include <mibiengine2/base/mat.h>
#include <mibiengine2/renderer/stdshader.h>

typedef struct {
    GEVec3 position;
    GEVec3 rotation;
    GEMat4 projection_mat;
    GEMat4 view_mat;
} GECamera;

typedef enum {
    GE_P_PERSPECTIVE,
    GE_P_ORTHOGRAPHIC,
    GE_P_AMOUNT
} GECameraType;

int ge_camera_init(GECamera *camera);

void ge_camera_perspective(GECamera *camera, float fov, float aspect_ratio,
                           float far, float near);

void ge_camera_orthographic(GECamera *camera, float left, float top,
                            float right, float bottom, float far, float near);

void ge_camera_update(GECamera *camera);

void ge_camera_set_position(GECamera *camera, float x, float y, float z);

void ge_camera_set_rotation(GECamera *camera, float x, float y, float z);

#define GE_CAMERA_ORTHO2D(camera, left, top, right, bottom) \
    ge_camera_orthographic(camera, left, top, right, bottom, 1, -1)

void ge_camera_use(GECamera *camera, GEStdShader *shader);

void ge_camera_free(GECamera *camera);

#endif

