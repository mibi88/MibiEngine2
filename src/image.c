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

#include <image.h>

#include <stdlib.h>

#include <png.h>

#define CHECK_NUM 8

int ge_image_load(GEImage *image, char *file) {
    png_byte header[CHECK_NUM];
    int is_png;

    png_structp png_ptr;
    png_infop info_ptr;

    png_uint_32 i;

    /* Check if the file is a png image. */
    FILE *fp = fopen(file, "rb");
    if(fp == NULL){
        return GE_IMAGE_ERROR_FILE;
    }
    fread(header, 1, CHECK_NUM, fp);
    is_png = !png_sig_cmp(header, 0, CHECK_NUM);
    if(!is_png){
        fclose(fp);
        return GE_IMAGE_ERROR_NOT_PNG;
    }

    /* Allocate memory. */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
                                                 NULL, NULL);
    if(png_ptr == NULL){
        fclose(fp);
        return GE_IMAGE_ERROR_OUT_OF_MEM;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if(info_ptr == NULL){
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return GE_IMAGE_ERROR_OUT_OF_MEM;
    }

    /* Initialize everything */
    if(setjmp(png_jmpbuf(png_ptr))){
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return GE_IMAGE_ERROR_UNKNOWN;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    /* Get some useful informations about the image */
    png_get_IHDR(png_ptr, info_ptr, &image->width, &image->height,
            &image->bit_depth, &image->color_type, NULL, NULL, NULL);

    /* Prepare for reading */
    if(image->color_type == PNG_COLOR_TYPE_PALETTE){
        png_set_expand(png_ptr);
    }
    if(image->color_type == PNG_COLOR_TYPE_GRAY && image->bit_depth < 8){
        png_set_expand(png_ptr);
    }
    if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)){
        png_set_expand(png_ptr);
    }

    png_read_update_info(png_ptr, info_ptr);

    /* Read! */
    image->rows = malloc(sizeof(png_bytep)*image->height);
    if(image->rows == NULL){
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return GE_IMAGE_ERROR_OUT_OF_MEM;
    }

    image->row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    image->data = malloc(image->height*image->row_bytes);
    if(image->data == NULL){
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        free(image->rows);
        return GE_IMAGE_ERROR_OUT_OF_MEM;
    }

    for(i=0;i<image->height;i++){
        image->rows[i] = image->data+i*image->row_bytes;
    }

    png_read_image(png_ptr, image->rows);

    png_read_end(png_ptr, NULL);

    /* Free everything */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    free(image->rows);

    fclose(fp);

    return GE_IMAGE_SUCCESS;
}

void ge_image_free(GEImage *image) {
    free(image->data);
}

