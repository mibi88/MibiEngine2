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

#ifndef GE_ENTITY_H
#define GE_ENTITY_H

#include <mibiengine2/base/mat.h>

typedef struct {
    GEVec3 position;
    GEVec3 velocity;
    GEVec3 rotation;
    GEVec3 scale;
    
    GEMat4 model_mat;
    GEMat3 normal_mat;
    void *data;
    
    char changed;
    void (*on_update)(void *_entity, void *_data);
    void *call_data;
} GEEntity;

int ge_entity_init(GEEntity *entity, void *data);

int ge_entity_set_position(GEEntity *entity, float x, float y, float z);

int ge_entity_set_rotation(GEEntity *entity, float x, float y, float z);

int ge_entity_set_scale(GEEntity *entity, float x, float y, float z);

int ge_entity_set_data(GEEntity *entity, void *data);

int ge_entity_set_update_callback(GEEntity *entity,
                                  void on_update(void *_entity, void *_data),
                                  void *data);

int ge_entity_update(GEEntity *entity);

void ge_entity_free(GEEntity *entity);

#endif

