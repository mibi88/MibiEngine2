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

#ifndef GE_STACK_H
#define GE_STACK_H

#include <stddef.h>

typedef struct {
    void *data;
    size_t size;
    size_t num;
    size_t max;
} GEStack;

/* ge_stack_init
 *
 * Create a new LIFO stack.
 *
 * stack:     The stack data.
 * max:       The max. number of items that the stack can contain.
 * item_size: The size of an item.
 * Return GE_E_NONE (0) on success or an error code on failure.
 */
int ge_stack_init(GEStack *stack, size_t max, size_t item_size);

/* ge_stack_push
 *
 * Push an item on the stack.
 *
 * stack: The stack data.
 * data:  A pointer to the data to push on the stack.
 * Return GE_E_NONE (0) on success or an error code on failure.
 */
int ge_stack_push(GEStack *stack, void *data);

/* ge_stack_pull
 *
 * Pull an item from the stack.
 *
 * stack: The stack data.
 * data:  A pointer that the pulled element from the stack will be written to.
 * Return GE_E_NONE (0) on success or an error code on failure.
 */
int ge_stack_pull(GEStack *stack, void *data);

/* ge_stack_free
 *
 * Free the stack data.
 *
 * stack: The stack data.
 */
void ge_stack_free(GEStack *stack);

#endif

