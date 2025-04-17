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

#include <mibiengine2/base/image.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <mibiengine2/errors.h>

#include <mibiengine2/config.h>

#define GE_IMAGE_PNG_HEADER_SIZE 8
#define GE_IMAGE_PNG_IHDR_SIZE 13

#if GE_IMAGE_USE_LIBPNG

#include <png.h>

int ge_image_init(GEImage *image, char *file) {
    png_byte header[GE_IMAGE_PNG_HEADER_SIZE];
    int is_png;

    png_structp png_ptr;
    png_infop info_ptr;

    png_uint_32 i;

    /* Check if the file is a png image. */
    FILE *fp = fopen(file, "rb");
    if(fp == NULL){
        return GE_E_FILE;
    }
    fread(header, 1, GE_IMAGE_PNG_HEADER_SIZE, fp);
    is_png = !png_sig_cmp(header, 0, GE_IMAGE_PNG_HEADER_SIZE);
    if(!is_png){
        fclose(fp);
        return GE_E_NOT_PNG;
    }

    /* Allocate memory. */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
                                                 NULL, NULL);
    if(png_ptr == NULL){
        fclose(fp);
        return GE_E_OUT_OF_MEM;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if(info_ptr == NULL){
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return GE_E_OUT_OF_MEM;
    }

    /* Initialize everything */
    if(setjmp(png_jmpbuf(png_ptr))){
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return GE_E_UNKNOWN;
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
        return GE_E_OUT_OF_MEM;
    }

    image->row_bytes = png_get_rowbytes(png_ptr, info_ptr);

    image->data = malloc(image->height*image->row_bytes);
    if(image->data == NULL){
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        free(image->rows);
        return GE_E_OUT_OF_MEM;
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

    return GE_E_NONE;
}

#else

#include <mibiengine2/base/deflate.h>

#define _GE_IMAGE_ERROR(rc) \
    { \
        free(data); \
        data = NULL; \
        ge_deflate_free(&deflate); \
        return rc; \
    }

int ge_image_init(GEImage *image, char *file) {
    unsigned char png_header[GE_IMAGE_PNG_HEADER_SIZE] = {
        0x89,
        0x50, 0x4E, 0x47,
        0x0D, 0x0A,
        0x1A,
        0x0A
    };
    
    unsigned char *data = NULL;
    size_t size, chunk_len;
    size_t i;
    size_t chunk = 0;
    FILE *fp;
    unsigned int w, h;
    unsigned char bit_depth;
    unsigned char color_type;
    unsigned char compression;
    unsigned char filter;
    unsigned char interlace;
    
    int ihdr_found = 0;
    int plte_found = 0;
    int iend_found = 0;
    
    char type[4];
    
    unsigned char palette[256*3];
    size_t palette_size;
    
    GEDeflate deflate;
    
    if(ge_deflate_init(&deflate)){
        return GE_E_DEFLATE_INIT;
    }
    
    fp = fopen(file, "r");
    if(fp == NULL){
        _GE_IMAGE_ERROR(GE_E_FILE);
    }
    
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);
    
    data = malloc(size);
    if(data == NULL){
        _GE_IMAGE_ERROR(GE_E_OUT_OF_MEM);
    }
    
    fread(data, 1, size, fp);
    
    printf("Read %lu bytes\n", size);
    
    if(size < GE_IMAGE_PNG_HEADER_SIZE){
        _GE_IMAGE_ERROR(GE_E_NOT_PNG);
    }
    
    if(memcmp(data, png_header, GE_IMAGE_PNG_HEADER_SIZE)){
        fputs("Invalid file header!\n", stderr);
        _GE_IMAGE_ERROR(GE_E_NOT_PNG);
    }
    
    i = GE_IMAGE_PNG_HEADER_SIZE;
    
    /* Read the chunks */
    
    while(i < size){
        /* Get the chunk length and type */
        if(i+4 > size){
            _GE_IMAGE_ERROR(GE_E_PNG_TOO_SMALL);
        }
        chunk_len = data[i]<<24 | data[i+1]<<16 | data[i+2]<<8 | data[i+3];
        i += 4;
        
        if(i+4 > size){
            _GE_IMAGE_ERROR(GE_E_PNG_TOO_SMALL);
        }
        memcpy(type, data+i, 4);
        i += 4;
        
        if(i+chunk_len+4 > size){
            fputs("Image data too small!\n", stderr);
            _GE_IMAGE_ERROR(GE_E_PNG_TOO_SMALL);
        }
        
        if(!memcmp(type, "IHDR", 4)){
            if(chunk_len != GE_IMAGE_PNG_IHDR_SIZE){
                fputs("Corrupted IHDR chunk!\n", stderr);
                _GE_IMAGE_ERROR(GE_E_CORRUPTED_IHDR);
            }
            
            w = data[i]<<24 | data[i+1]<<16 | data[i+2]<<8 | data[i+3];
            i += 4;
            
            h = data[i]<<24 | data[i+1]<<16 | data[i+2]<<8 | data[i+3];
            i += 4;
            
            bit_depth = data[i++];
            color_type = data[i++];
            compression = data[i++];
            filter = data[i++];
            interlace = data[i++];
            
            printf("IHDR: %u, %u - %u, %u, %u, %u, %u\n", w, h, bit_depth,
                   color_type, compression, filter, interlace);
            
            if((bit_depth&(bit_depth-1)) || bit_depth > 8){
                fputs("Invalid bit depth!\n", stderr);
                _GE_IMAGE_ERROR(GE_E_INVALID_BIT_DEPTH);
            }
            
            ihdr_found = 1;
        }else if(!memcmp(type, "PLTE", 4)){
            puts("Palette found!");
            if(chunk_len > (size_t)((1<<bit_depth)*3) || chunk_len%3){
                puts("Bad palette size");
                _GE_IMAGE_ERROR(GE_E_CORRUPTED_PLTE);
            }
            palette_size = chunk_len/3;
            memcpy(palette, data+i, chunk_len);
            i += chunk_len;
        }else if(!memcmp(type, "IDAT", 4)){
            puts("Image data found!");
            if(i+chunk_len > size){
                fputs("Image too small!\n", stderr);
                _GE_IMAGE_ERROR(GE_E_PNG_TOO_SMALL);
            }
            printf("i: %d\n", i);
            if(ge_deflate_decompress(&deflate, data+i, chunk_len)){
                fputs("Decompression failed!\n", stderr);
                _GE_IMAGE_ERROR(GE_E_DECOMPRESSION_FAILED);
            }
            i += chunk_len;
        }else if(!memcmp(type, "IEND", 4)){
            puts("Image end found!");
            /* The IEND chunk should be empty */
            if(chunk_len != 0){
                _GE_IMAGE_ERROR(GE_E_CORRUPTED_IEND);
            }
            iend_found = 1;
        }else{
            if(isupper(type[0])){
                /* It is a critical chunk */
                fputs("Unknown critical chunk!\n", stderr);
                _GE_IMAGE_ERROR(GE_E_UNKNOWN_CRITICAL_CHUNK);
            }
            if(isupper(type[1])){
                /* It is a standardized chunk */
            }
            i += chunk_len;
        }
        
        /* The IHDR chunk should be the first chunk */
        if(!chunk && !ihdr_found){
            fputs("IHDR chunk not found!\n", stderr);
            _GE_IMAGE_ERROR(GE_E_IHDR_NOT_FOUND);
        }
        
        /* Check if the checksum is correct */
        /* TODO */
        i += 4;
        
        chunk++;
        
        /* The IEND chunk marks the end of a PNG file */
        if(iend_found) break;
    }
    
    free(data);
    ge_deflate_free(&deflate);
    
    return GE_E_UNKNOWN;
}

#endif

int ge_image_empty(GEImage *image, int width, int height) {
    image->data = calloc(width*height*4, 1);
    if(image->data == NULL){
        return GE_E_OUT_OF_MEM;
    }
    image->width = width;
    image->height = height;
    image->row_bytes = width*4;
    return GE_E_NONE;
}

void ge_image_free(GEImage *image) {
    free(image->data);
    image->data = NULL;
}

