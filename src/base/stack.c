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

#include <mibiengine2/base/stack.h>
#include <mibiengine2/errors.h>

#include <stdlib.h>
#include <string.h>

int ge_stack_init(GEStack *stack, size_t max, size_t item_size) {
    stack->data = malloc(max*item_size);
    if(stack->data == NULL){
        return GE_E_OUT_OF_MEM;
    }
    
    stack->size = item_size;
    stack->num = 0;
    stack->max = max;
    
    return GE_E_NONE;
}

int ge_stack_push(GEStack *stack, void *data) {
    if(stack->num >= stack->max) return GE_E_STACK_OVERFLOW;
    memcpy((char*)stack->data+(stack->num++)*stack->size, data, stack->size);
    return GE_E_NONE;
}

int ge_stack_pull(GEStack *stack, void *data) {
    if(!stack->num) return GE_E_STACK_UNDERFLOW;
    memcpy(data, (char*)stack->data+(--stack->num)*stack->size, stack->size);
    return GE_E_NONE;
}

void ge_stack_free(GEStack *stack) {
    free(stack->data);
    stack->data = NULL;
}

