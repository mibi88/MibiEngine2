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

#include <mibiengine2/base/deflate.h>
#include <mibiengine2/errors.h>

#include <stdio.h>

int ge_deflate_init(GEDeflate *deflate) {
    deflate->output = NULL;
    deflate->output_size = 0;
    deflate->cur = 0;
    deflate->header_found = 0;
    deflate->adler32_found = 0;
    return GE_E_NONE;
}

int ge_deflate_decompress(GEDeflate *deflate, unsigned char *data, int size) {
    size_t i = 0;
    if(!deflate->cur){
        /* The data is too small */
        if(size-i < 1) return GE_E_NONE;
        /* Read the compression method and compression info byte */
        /* Get the 4 first bits */
        deflate->cmf = data[i];
        deflate->compression.method = data[i]&0xF;
        if(deflate->compression.method != 8){
            fputs("Unsupported compression method!\n", stderr);
            return GE_E_UNSUPPORTED_COMPRESSION;
        }
        /* Get the 4 last bits */
        deflate->compression.info = (data[i]>>8);
        deflate->window_size = 1<<(deflate->compression.info+8);
        if(deflate->compression.info > 7){
            fputs("Invalid window size!\n", stderr);
            /* The current ZLIB specification doesn't allow window sizes bigger
             * than 7. */
            return GE_E_INVALID_WINDOW_SIZE;
        }
        printf("LZ77 window size: %lu\n", deflate->window_size);
        deflate->cur++;
        i++;
    }
    if(deflate->cur == 1){
        /* The data is too small */
        if(size-i < 1) return GE_E_NONE;
        /* Read the flags */
        /* Get the 6th bit */
        deflate->has_dict = data[i]&(1<<5);
        /* Get the last two bits */
        deflate->compression.level = data[i]&((1<<6)|(1<<7));
        deflate->dict_check = 0;
        deflate->cur++;
        i++;
    }
    if(deflate->cur >= 2 && deflate->cur < 6 && deflate->has_dict){
        /* Read the dictionary Adler-32 checksum */
        for(;deflate->cur<6;deflate->cur++,i++){
            /* The data is too small */
            if(size-i < 1) return GE_E_NONE;
            deflate->dict_check |= data[i]<<((deflate->cur-1)*8);
        }
    }
    /* Decompress the data */
    if((deflate->cur >= 2 && !deflate->has_dict) ||
       (deflate->cur >= 6 && deflate->has_dict)){
        /*  */
    }
    
    /* Check the integrity of the data with the Adler-32 checksum. */
    return GE_E_NONE;
}

void ge_deflate_free(GEDeflate *deflate) {
    /* TODO */
    (void)deflate;
}

