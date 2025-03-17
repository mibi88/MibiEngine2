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

#ifndef GE_MAT_H
#define GE_MAT_H

typedef struct {
    float mat[4*4];
} GEMat4;

typedef struct {
    float mat[3*3];
} GEMat3;

typedef struct {
    float x, y, z, w;
} GEVec4;

typedef struct {
    float x, y;
} GEVec2;

typedef enum {
    A_X,
    A_Y,
    A_Z
} GEAxis;

void ge_mat4_identity(GEMat4 *mat);
void ge_mat4_mmul(GEMat4 *dest, GEMat4 *src1, GEMat4 *src2);
void ge_mat4_mvmul(GEVec4 *dest, GEMat4 *mat, GEVec4 *vec);
void ge_mat4_vmmul(GEVec4 *dest, GEVec4 *vec, GEMat4 *mat);
void ge_mat4_transpose(GEMat4 *dest, GEMat4 *src);
void ge_mat4_inverse(GEMat4 *dest, GEMat4 *src);
void ge_mat4_rot3d(GEMat4 *mat, GEAxis axis, float angle);
void ge_mat4_translate3d(GEMat4 *mat, float x, float y, float z);
void ge_mat4_scale3d(GEMat4 *mat, float x, float y, float z);
void ge_mat4_projection3d(GEMat4 *mat, float fov, float aspect_ratio,
                          float far, float near);
void ge_mat3_mat4(GEMat3 *mat3, GEMat4 *mat4);

#endif

