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

#include <mibiengine2/renderer/camera.h>
#include <mibiengine2/errors.h>

int ge_camera_init(GECamera *camera) {
    camera->position.x = 0;
    camera->position.y = 0;
    camera->position.z = 0;
    
    camera->rotation.x = 0;
    camera->rotation.y = 0;
    camera->rotation.z = 0;
    
    ge_camera_update(camera);
    ge_mat4_identity(&camera->projection_mat);
    return GE_E_SUCCESS;
}

void ge_camera_perspective(GECamera *camera, float fov, float aspect_ratio,
                           float far, float near) {
    ge_mat4_projection3d(&camera->projection_mat, fov, aspect_ratio, far,
                         near);
}

void ge_camera_orthographic(GECamera *camera, float left, float top,
                            float right, float bottom, float far, float near) {
    ge_mat4_ortho3d(&camera->projection_mat, left, top, right, bottom, far,
                    near);
}

void ge_camera_update(GECamera *camera) {
    GEMat4 tmp1, tmp2, tmp3;
    ge_mat4_rot3d(&tmp1, GE_A_X, camera->rotation.x);
    ge_mat4_rot3d(&tmp2, GE_A_Y, camera->rotation.y);
    ge_mat4_mmul(&tmp3, &tmp1, &tmp2);
    ge_mat4_rot3d(&tmp1, GE_A_Z, camera->rotation.z);
    ge_mat4_mmul(&tmp2, &tmp1, &tmp3);
    
    ge_mat4_translate3d(&tmp1, camera->position.x, camera->position.y,
                        camera->position.z);
    ge_mat4_mmul(&camera->view_mat, &tmp1, &tmp2);
}

void ge_camera_use(GECamera *camera, GEStdShader *shader) {
    ge_shader_load_mat4(&shader->projection_mat, &camera->projection_mat);
    ge_shader_load_mat4(&shader->view_mat, &camera->view_mat);
}

void ge_camera_set_position(GECamera *camera, float x, float y, float z) {
    camera->position.x = x;
    camera->position.y = y;
    camera->position.z = z;
}

void ge_camera_set_rotation(GECamera *camera, float x, float y, float z) {
    camera->rotation.x = x;
    camera->rotation.y = y;
    camera->rotation.z = z;
}

void ge_camera_free(GECamera *camera) {
    (void)camera;
    /* Nothing to do here */
}

