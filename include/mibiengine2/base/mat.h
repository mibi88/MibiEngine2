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
    float x, y, z;
} GEVec3;

typedef struct {
    float x, y;
} GEVec2;

typedef enum {
    GE_A_X,
    GE_A_Y,
    GE_A_Z
} GEAxis;

/* ge_mat4_identity
 *
 * Set a 4x4 matrix to an identity matrix.
 *
 * mat: The matrix set to an identity matrix.
 */
void ge_mat4_identity(GEMat4 *mat);

/* ge_mat4_mmul
 *
 * Multiply a 4x4 matrix by another 4x4 matrix.
 *
 * dest: The destination matrix (must be different to src1 and src2).
 * src1: A 4x4 matrix that will be multiplied with src2.
 * src2: A 4x4 matrix that will be multiplied with src1.
 */
void ge_mat4_mmul(GEMat4 *dest, GEMat4 *src1, GEMat4 *src2);

/* ge_mat4_mvmul
 *
 * Multiply a 4x4 matrix by a vector (matrix multiplication isn't commutative).
 *
 * dest: The destination vector.
 * mat:  The matrix to multiply the vector with.
 * vec:  The vector to multiply the matrix with.
 */
void ge_mat4_mvmul(GEVec4 *dest, GEMat4 *mat, GEVec4 *vec);

/* ge_mat4_vmmul
 *
 * Multiply a vector by a 4x4 matrix (matrix multiplication isn't commutative).
 *
 * dest: The destination vector.
 * vec:  The vector to multiply the matrix with.
 * mat:  The matrix to multiply the vector with.
 */
void ge_mat4_vmmul(GEVec4 *dest, GEVec4 *vec, GEMat4 *mat);

/* ge_mat4_transpose
 *
 * Transpose a 4x4 matrix.
 *
 * dest: The transposed matrix.
 * src:  The matrix to transpose.
 */
void ge_mat4_transpose(GEMat4 *dest, GEMat4 *src);

/* ge_mat4_inverse
 *
 * Invert a 4x4 matrix.
 *
 * dest: The destination matrix.
 * src:  The matrix to invert.
 */
void ge_mat4_inverse(GEMat4 *dest, GEMat4 *src);

/* ge_mat4_rot3d
 *
 * Create a 3D 4x4 rotation transformation matrix.
 *
 * matrix: The rotation matrix.
 * axis:   The axis to rotate around (see GEAxis).
 * angle:  The angle in degrees of the rotation.
 */
void ge_mat4_rot3d(GEMat4 *mat, GEAxis axis, float angle);

/* ge_mat4_translate3d
 *
 * Create a 4x4 3D translation transformation matrix.
 *
 * mat: The destination matrix.
 * x:   The translation on the X axis.
 * y:   The translation on the Y axis.
 * z:   The translation on the Z axis.
 */
void ge_mat4_translate3d(GEMat4 *mat, float x, float y, float z);

/* ge_mat4_scale3d
 *
 * Create a 4x4 3D scaling transformation matrix.
 *
 * mat: The destination matrix.
 * x:   The scale on the X axis.
 * y:   The scale on the Y axis.
 * z:   The scale on the Z axis.
 */
void ge_mat4_scale3d(GEMat4 *mat, float x, float y, float z);

/* ge_mat4_projection3d
 *
 * Create a 3D 4x4 perspective projection matrix.
 *
 * mat:          The destination matrix.
 * fov:          The field of view.
 * aspect_ratio: The aspect ratio (width/height).
 * far:          The distance of the far plane.
 * near:         The distance of the near plane.
 */
void ge_mat4_projection3d(GEMat4 *mat, float fov, float aspect_ratio,
                          float far, float near);

/* ge_mat3_mat4
 *
 * Cast a 4x4 matrix to a 3x3 matrix (the top left half will be kept).
 *
 * mat3: The destination 3x3 matrix.
 * mat4: The 4x4 matrix.
 */
void ge_mat3_mat4(GEMat3 *mat3, GEMat4 *mat4);

#endif

