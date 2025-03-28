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

#include <mibiengine2/base/arena.h>
#include <mibiengine2/errors.h>

#include <stdlib.h>

int ge_arena_init(GEArena *arena, size_t alloc_step, size_t max) {
    arena->ptr = malloc(max);
    if(arena->ptr == NULL){
        return GE_E_OUT_OF_MEM;
    }
    arena->max = max;
    arena->alloc_step = alloc_step;
    arena->size = 0;
    return GE_E_SUCCESS;
}

void *ge_arena_alloc(GEArena *arena, size_t size) {
    size_t added;
    size_t align = 0;
    size_t c;
    void *new;
    if(arena->size%size) align = size-(arena->size%size);
    added = size+align;
    if(arena->size+added >= arena->max){
        c = added/arena->alloc_step+(added%arena->alloc_step ? 1 : 0);
        new = realloc(arena->ptr, arena->max+c*arena->alloc_step);
        if(new == NULL){
            return NULL;
        }
        arena->ptr = new;
        arena->max += c*arena->alloc_step;
    }
    new = (char*)arena->ptr+arena->size+align;
    arena->size += added;
    return new;
}

void ge_arena_free(GEArena *arena) {
    free(arena->ptr);
    arena->ptr = NULL;
    arena->max = 0;
    arena->size = 0;
}

