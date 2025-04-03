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

#ifndef GE_IMAGE_H
#define GE_IMAGE_H

typedef struct {
    /* TODO: Only store the width, height, data and maybe the color format. */
    unsigned int width, height;
    int bit_depth;
    int color_type;
    unsigned char *data;
    unsigned char **rows;
    long row_bytes;
} GEImage;

/* ge_image_init
 *
 * Load an image.
 *
 * image: The image data.
 * file:  The file name of the image.
 * Returns GE_E_NONE (0) on success or an error code on failure.
 */
int ge_image_init(GEImage *image, char *file);

/* ge_image_empty
 *
 * Create a new empty image filled with the RGBA color 0, 0, 0, 0.
 *
 * image: The image data.
 * Returns GE_E_NONE (0) on success or an error code on failure.
 */
int ge_image_empty(GEImage *image, int width, int height);

/* GE_IMAGE_GET_PIXEL_PTR
 *
 * Get a pointer to the pixel at x, y where an RGBA value can be written to.
 * It is made up of unsigned chars. If ptr is the pointer, ptr[0] is red,
 * ptr[1] is green, ptr[2] is blue and ptr[3] alpha.
 *
 * image: A pointer to the image data.
 * x:     The position of the pixel on the X axis.
 * y:     The position of the pixel on the Y axis.
 */
#define GE_IMAGE_GET_PIXEL_PTR(image, x, y) (image->data+(y*image->width+x)*4)

/* GE_IMAGE_GET_WIDTH
 *
 * Get the width of an image.
 *
 * image: A pointer to the image data.
 */
#define GE_IMAGE_GET_WIDTH(image) (image->width)

/* GE_IMAGE_GET_HEIGHT
 *
 * Get the height of an image.
 *
 * image: A pointer to the image data.
 */
#define GE_IMAGE_GET_HEIGHT(image) (image->height)

/* ge_image_free
 *
 * Free an image.
 *
 * image: The image to free.
 */
void ge_image_free(GEImage *image);

#endif

