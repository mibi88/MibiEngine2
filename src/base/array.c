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

#include <mibiengine2/base/array.h>
#include <mibiengine2/errors.h>
#include <stdlib.h>
#include <string.h>

int ge_array_init(GEArray *array, size_t count, size_t item_size,
                  void *items) {
    if(!count || items == NULL){
        array->ptr = NULL;
        count = 0;
    }else{
        array->ptr = malloc(count*item_size);
        if(array->ptr == NULL){
            return GE_E_OUT_OF_MEM;
        }
        memcpy(array->ptr, items, count*item_size);
    }

    array->count = count;
    array->item_size = item_size;
    return GE_E_NONE;
}

int ge_array_add(GEArray *array, void *items, size_t count) {
    void *new;

    new = realloc(array->ptr, (array->count+count)*array->item_size);
    if(new == NULL){
        return GE_E_OUT_OF_MEM;
    }

    array->ptr = new;

    memcpy((char*)array->ptr+array->count*array->item_size, items,
           count*array->item_size);

    array->count += count;

    return GE_E_NONE;
}

void ge_array_free(GEArray *array) {
    free(array->ptr);
    array->ptr = NULL;
}
