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

#include <mibiengine2/base/shadertree.h>
#include <mibiengine2/errors.h>

#include <string.h>

/* NOTE: For testing only */
#include <stdio.h>

int ge_shadertree_init(GEShaderTree *tree) {
    if(ge_arena_init(&tree->tokens, GE_SHADERTREE_ALLOC_STEP,
                     GE_SHADERTREE_ALLOC_STEP)){
        return GE_E_ARENA_INIT;
    }
    if(ge_arena_init(&tree->token_strings, GE_SHADERTREE_ALLOC_STEP,
                     GE_SHADERTREE_ALLOC_STEP)){
        ge_arena_free(&tree->tokens);
        return GE_E_ARENA_INIT;
    }
    if(ge_arena_init(&tree->token_types, GE_SHADERTREE_ALLOC_STEP,
                     GE_SHADERTREE_ALLOC_STEP)){
        ge_arena_free(&tree->tokens);
        ge_arena_free(&tree->token_strings);
        return GE_E_ARENA_INIT;
    }
    if(ge_arena_init(&tree->macro_expansions, GE_SHADERTREE_ALLOC_STEP,
                     GE_SHADERTREE_ALLOC_STEP)){
        ge_arena_free(&tree->tokens);
        ge_arena_free(&tree->token_strings);
        ge_arena_free(&tree->token_types);
        return GE_E_ARENA_INIT;
    }
    if(ge_arena_init(&tree->comments, GE_SHADERTREE_ALLOC_STEP,
                     GE_SHADERTREE_ALLOC_STEP)){
        ge_arena_free(&tree->tokens);
        ge_arena_free(&tree->token_strings);
        ge_arena_free(&tree->token_types);
        ge_arena_free(&tree->macro_expansions);
        return GE_E_ARENA_INIT;
    }
    tree->token_num = 0;
    return GE_E_NONE;
}

int ge_shadertree_load(GEShaderTree *tree, char *shader) {
    int rc;
    if((rc = ge_shadertree_preprocessor(tree, shader))) return rc;
    if((rc = ge_shadertree_tokenize(tree, shader))) return rc;
    if((rc = ge_shadertree_generate(tree))) return rc;
    return GE_E_NONE;
}

#define LAST_CHAR_NUM 8

int ge_shadertree_preprocessor(GEShaderTree *tree, char *shader) {
    size_t i;
    size_t len;
    int in_comment = 0;
    int crlf = 0;
    int line_started = 0;
    int in_directive = 0;
    char last_chars[LAST_CHAR_NUM];
    size_t last_char_num = 0;
    size_t line = 1;
    size_t comment_idx = 0;
    size_t comment_num = 0;
    size_t start_line;
    char spaces[] = " \t";
    char directive_args[GE_SHADERTREE_DEFINE_ARGS_MAX][GE_SHADERTREE_ARG_SZ];
    GEShaderTreeComment comment;
    GEShaderTreeComment *ptr;
    GEShaderTreeComment *comments;
    crlf = strchr(shader, '\r') != NULL;
    printf("CRLF: %d\n", crlf);
    len = strlen(shader);
    
    (void)line_started;
    (void)directive_args;
    
    comments = tree->comments.ptr;
    
    /* Search all the comments. */
    for(i=0;i<len;i++){
        /* Load the current char */
        if(last_char_num < LAST_CHAR_NUM){
            last_chars[last_char_num++] = shader[i];
        }else{
            memmove(last_chars, last_chars+1, LAST_CHAR_NUM-1);
            last_chars[LAST_CHAR_NUM-1] = shader[i];
        }
        /*fwrite(last_chars, last_char_num, 1, stdout);
        puts("");*/
        
        /* Check if we are in a comment, or if we exit one */
        if(last_char_num >= 2){
            if(in_comment && !memcmp(last_chars+last_char_num-2, "*/", 2)){
                in_comment = 0;
                comment.end = i+1;
                comment.lines = line-start_line;
                ptr = ge_arena_alloc(&tree->comments, 1,
                                     sizeof(GEShaderTreeComment));
                if(ptr == NULL){
                    return GE_E_ARENA_ALLOC;
                }
                *ptr = comment;
                comment_num++;
            }else if(!in_comment && !memcmp(last_chars+last_char_num-2, "/*",
                                            2)){
                in_comment = 1;
                comment.start = i-1;
                start_line = line;
            }
        }
        
        /* Increment the line number */
        if(crlf && !memcmp(last_chars+last_char_num-2, "\r\n", 2)){
            line++;
        }else if(!crlf && last_chars[last_char_num-1] == '\n'){
            line++;
        }
    }
    
    line = 1;
    line_started = 0;
    
    printf("Comment num: %lu\n", comment_num);
    
    for(i=0;i<len;i++){
        /* Skip comments */
        if(comment_idx < comment_num){
            if(i == comments[comment_idx].start){
                i = comments[comment_idx].end;
                line += comments[comment_idx].lines;
                comment_idx++;
            }
        }
        /* Load the current char */
        if(last_char_num < LAST_CHAR_NUM){
            last_chars[last_char_num++] = shader[i];
        }else{
            memmove(last_chars, last_chars+1, LAST_CHAR_NUM-1);
            last_chars[LAST_CHAR_NUM-1] = shader[i];
        }
        
        /*putc(shader[i], stdout);*/
        
        /* Search for preprocessor directives */
        if(in_directive){
            /* Load the directive */
        }
        if(strchr(spaces, shader[i]) == NULL){
            line_started = 1;
            if(shader[i] == '#'){
                printf("Preprocessor directive found line %lu\n", line);
                in_directive = 1;
            }
        }
        
        /* Increment the line number */
        if(crlf && !memcmp(last_chars+last_char_num-2, "\r\n", 2)){
            line++;
            line_started = 0;
        }else if(!crlf && last_chars[last_char_num-1] == '\n'){
            line++;
            line_started = 0;
        }
    }
    printf("Lines: %lu\n", line);
    return GE_E_NONE;
}

int ge_shadertree_tokenize(GEShaderTree *tree, char *shader) {
    (void)tree;
    (void)shader;
    return GE_E_NONE;
}

int ge_shadertree_generate(GEShaderTree *tree) {
    (void)tree;
    return GE_E_NONE;
}

void ge_shadertree_free(GEShaderTree *tree) {
    ge_arena_free(&tree->tokens);
    ge_arena_free(&tree->token_strings);
    ge_arena_free(&tree->token_types);
    ge_arena_free(&tree->macro_expansions);
    ge_arena_free(&tree->comments);
}

