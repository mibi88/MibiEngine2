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

#ifndef GE_ARENA_H
#define GE_ARENA_H

#include <stddef.h>

typedef struct {
    void *ptr;
    size_t max;
    size_t size;
    size_t alloc_step;
} GEArena;

/* ge_arena_init
 *
 * Create a new arena allocator.
 *
 * arena:      The arena allocator data.
 * alloc_step: The number of bytes to allocate when extending the array.
 * max:        The numbers of bytes to allocate on initialization for later
 *             use.
 * Returns GE_E_SUCCESS (0) on success or an error code on failure.
 */
int ge_arena_init(GEArena *arena, size_t alloc_step, size_t max);

/* ge_arena_alloc
 *
 * Allocate some memory.
 *
 * arena: The arena allocator data.
 * size:  The number of bytes to allocate.
 * Returns a pointer to the allocated data or NULL on failure.
 */
void *ge_arena_alloc(GEArena *arena, size_t size);

/* ge_arena_free
 *
 * Free the entire arena.
 *
 * arena: The arena allocator data.
 */
void ge_arena_free(GEArena *arena);

#endif

