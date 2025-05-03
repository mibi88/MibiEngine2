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

#include <mibiengine2/base/utils.h>
#include <mibiengine2/config.h>
#include <mibiengine2/errors.h>

#include <stdlib.h>
#include <string.h>

int ge_utils_power_of_two(int num) {
    /* Find the closest power of two */
    int c;
    if(!(num&(num-1))){
        /* It already is a power of two */
        return num;
    }
    for(c=1;num>>=1;c++);
    return 1<<c;
}

/* longs are at least 32bits, ints can be smaller */
unsigned long int ge_utils_adler32(unsigned char *data, size_t size) {
    unsigned short int s1 = 1;
    unsigned short int s2 = 0;
    size_t i;
    
    for(i=0;i<size;i++){
        s1 += data[i];
        s2 += s1;
        s1 &= 0xFFFF;
        s2 &= 0xFFFF;
    }
    return (s2<<16)|s1;
}

int ge_utils_sort(void *data, size_t size, size_t item_size,
                  int cmp(const void *item1, const void *item2)) {
    size_t i, n;
    void *tmp = malloc(item_size);
    if(tmp == NULL) return 1;
    for(i=1;i<size;i++){
        for(n=i;n ? n-- : n;){
            if(cmp((unsigned char*)data+i*item_size,
                   (unsigned char*)data+n*item_size) <= 0){
                n++;
                break;
            }
        }
        memcpy(tmp, (unsigned char*)data+i*item_size, item_size);
        memmove((unsigned char*)data+n*item_size+item_size,
                (unsigned char*)data+n*item_size, (i-n)*item_size);
        memcpy((unsigned char*)data+n*item_size, tmp, item_size);
    }
    return GE_E_NONE;
}

