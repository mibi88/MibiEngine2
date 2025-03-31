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

#ifndef GE_SHADERTREE_H
#define GE_SHADERTREE_H

#include <mibiengine2/base/arena.h>

#define GE_SHADERTREE_ALLOC_STEP 512

typedef struct {
    GEArena tokens;
    GEArena token_strings;
    GEArena token_types;
    GEArena macro_expansions;
    GEArena comments;
    size_t token_num;
    size_t error_line;
    unsigned char error_code;
} GEShaderTree;

typedef struct {
    long int changedlines;
    size_t start_line;
} GEShaderTreeMacroExpansion;

typedef struct {
    size_t start;
    size_t end;
    size_t lines;
} GEShaderTreeComment;

/* Shader compilation error codes */
enum {
    GE_S_E_NONE,
    GE_S_E_AMOUNT
};

int ge_shadertree_init(GEShaderTree *tree);

int ge_shadertree_load(GEShaderTree *tree, char *shader);

int ge_shadertree_preprocessor(GEShaderTree *tree, char *shader);

int ge_shadertree_tokenize(GEShaderTree *tree, char *shader);

int ge_shadertree_generate(GEShaderTree *tree);

void ge_shadertree_free(GEShaderTree *tree);

#endif

