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

#ifndef GE_SCENE_H
#define GE_SCENE_H

#include <mibiengine2/base/arena.h>

#include <mibiengine2/renderer/renderable.h>
#include <mibiengine2/renderer/entity.h>
#include <mibiengine2/renderer/camera.h>

#define GE_SCENE_ALLOC_STEP 512

typedef struct {
    GEArena model_mat;
    GEArena normal_mat;
    GEArena entities;
    GERenderable *renderable;
    size_t entity_num;
} GESceneEntityGroup;

typedef struct {
    GEArena entity_groups;
    size_t entity_group_num;
    GEStdShader **shaders;
    size_t shader_num;
    GECamera *camera;
    size_t light_max;
} GEScene;

int ge_scene_init(GEScene *scene, GEEntity *entities, size_t entity_num,
                  GEStdShader **shaders, size_t shader_num, size_t light_max);

int ge_scene_add_entities(GEScene *scene, GEEntity *entites,
                          size_t entity_num);

void ge_scene_set_shaders(GEScene *scene, GEStdShader **shaders,
                          size_t shader_num);

void ge_scene_set_camera(GEScene *scene, GECamera *camera);

GEEntity *ge_scene_get_same_entity(GEScene *scene, GEEntity *entity);

void ge_scene_for_entity(GEScene *scene,
                         void on_entity(GEEntity *entity, void *data),
                         void *data);

void ge_scene_for_entity_with_renderable(GEScene *scene,
                                         GERenderable *renderable,
                                         void on_entity(GEEntity *entity,
                                                        void *data),
                                         void *data);

void ge_scene_update(GEScene *scene);

void ge_scene_render(GEScene *scene);

void ge_scene_free(GEScene *scene);

#endif

