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

#include <mibiengine2/renderer/entity.h>

int ge_entity_init(GEEntity *entity, void *data) {
    entity->data = data;
    entity->scale.x = 1;
    entity->scale.y = 1;
    entity->scale.z = 1;
    return 0;
}

int ge_entity_set_position(GEEntity *entity, float x, float y, float z) {
    entity->position.x = x;
    entity->position.y = y;
    entity->position.z = z;
    return 0;
}

int ge_entity_set_rotation(GEEntity *entity, float x, float y, float z) {
    entity->rotation.x = x;
    entity->rotation.y = y;
    entity->rotation.z = z;
    return 0;
}

int ge_entity_set_scale(GEEntity *entity, float x, float y, float z) {
    entity->scale.x = x;
    entity->scale.y = y;
    entity->scale.z = z;
    return 0;
}

int ge_entity_set_data(GEEntity *entity, void *data) {
    entity->data = data;
    return 0;
}

int ge_entity_update(GEEntity *entity) {
    ge_mat4_translate3d(&entity->mat, entity->position.x, entity->position.y,
                        entity->position.z);
    ge_mat4_rot3d(&entity->mat, GE_A_X, entity->rotation.x);
    ge_mat4_rot3d(&entity->mat, GE_A_Y, entity->rotation.y);
    ge_mat4_rot3d(&entity->mat, GE_A_Z, entity->rotation.z);
    ge_mat4_scale3d(&entity->mat, entity->scale.x, entity->scale.y,
                    entity->scale.z);
    /* TODO: Create the normal matrix */
    ge_mat3_mat4(&entity->normal_mat, &entity->mat);
    return 0;
}

void ge_entity_free(GEEntity *entity) {
    (void)entity;
    /* Nothing needs to be done */
}

