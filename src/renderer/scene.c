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

#include <mibiengine2/renderer/scene.h>

#include <mibiengine2/renderer/light.h>
#include <mibiengine2/renderer/entity.h>

#include <mibiengine2/errors.h>

#include <stdlib.h>

void _ge_scene_entity_update(void *_entity, void *_data) {
    size_t i;
    size_t pos;
    /* TODO: Optimize this */
    GEScene *scene = _data;
    GEEntity *entity = _entity;
    GESceneEntityGroup *group;
    for(i=0;i<scene->entity_group_num;i++){
        group = ((GESceneEntityGroup*)scene->entity_groups.ptr)+i;
        if(entity >= (GEEntity*)group->entities.ptr &&
           entity < (GEEntity*)group->entities.ptr+group->entities.size){
            /* Update the model and normal matrices */
            pos = (entity-(GEEntity*)group->entities.ptr)/sizeof(GEEntity);
            ((GEMat4*)group->model_mat.ptr)[pos] = entity->model_mat;
            ((GEMat3*)group->normal_mat.ptr)[pos] = entity->normal_mat;
            break;
        }
    }
}

int ge_scene_init(GEScene *scene, GEEntity *entities, size_t entity_num,
                  size_t light_max) {
    size_t i, n;
    int found;
    GESceneEntityGroup *group;
    void *new;
    /* TODO: Optimize this */
    scene->entity_group_num = 0;
    /* Initialize the entity group arena */
    if(ge_arena_init(&scene->entity_groups, GE_SCENE_ALLOC_STEP,
                     GE_SCENE_ALLOC_STEP)){
        ge_scene_free(scene);
        return GE_E_ARENA_INIT;
    }
    /* Add all the entities and renderables. Store the entity matrices
     * separately for faster rendering */
    for(i=0;i<entity_num;i++){
        /* Search if the renderable is already stored */
        found = 0;
        for(n=0;n<scene->entity_group_num;n++){
            group = ((GESceneEntityGroup*)scene->entity_groups.ptr)+n;
            if(group->renderable == entities[i].data){
                found = 1;
                break;
            }
        }
        if(!found){
            /* Add an entity group */
            group = ge_arena_alloc(&scene->entity_groups,
                                   sizeof(GESceneEntityGroup));
            group->entity_num = 0;
            group->renderable = entities[i].data;
            if(ge_arena_init(&group->model_mat, GE_SCENE_ALLOC_STEP,
                             GE_SCENE_ALLOC_STEP)){
                ge_scene_free(scene);
                return GE_E_ARENA_INIT;
            }
            if(ge_arena_init(&group->normal_mat, GE_SCENE_ALLOC_STEP,
                             GE_SCENE_ALLOC_STEP)){
                ge_arena_free(&group->model_mat);
                ge_scene_free(scene);
                return GE_E_ARENA_INIT;
            }
            if(ge_arena_init(&group->entities, GE_SCENE_ALLOC_STEP,
                             GE_SCENE_ALLOC_STEP)){
                ge_arena_free(&group->model_mat);
                ge_arena_free(&group->normal_mat);
                ge_scene_free(scene);
                return GE_E_ARENA_INIT;
            }
            scene->entity_group_num++;
        }
        /* Add the entity to the entity group pointed to by group */
        new = ge_arena_alloc(&group->model_mat, sizeof(GEMat4));
        if(new == NULL){
            ge_scene_free(scene);
            return GE_E_ARENA_ALLOC;
        }
        *(GEMat4*)new = entities[i].model_mat;
        new = ge_arena_alloc(&group->normal_mat, sizeof(GEMat3));
        if(new == NULL){
            ge_scene_free(scene);
            return GE_E_ARENA_ALLOC;
        }
        *(GEMat3*)new = entities[i].normal_mat;
        new = ge_arena_alloc(&group->entities, sizeof(GEEntity));
        if(new == NULL){
            ge_scene_free(scene);
            return GE_E_ARENA_ALLOC;
        }
        *(GEEntity*)new = entities[i];
        /* Set the update callback of the newly updated entity */
        ge_entity_set_update_callback(new, _ge_scene_entity_update, scene);
        group->entity_num++;
    }
    scene->light_max = light_max;
    return GE_E_SUCCESS;
}

void ge_scene_render(GEScene *scene) {
    size_t i;
    GESceneEntityGroup *group;
    for(i=0;i<scene->entity_group_num;i++){
        group = ((GESceneEntityGroup*)scene->entity_groups.ptr)+i;
        ge_renderable_render_multiple(group->renderable, group->model_mat.ptr,
                                      group->normal_mat.ptr,
                                      group->entity_num);
    }
}

void ge_scene_free(GEScene *scene) {
    size_t i;
    GESceneEntityGroup *group;
    for(i=0;i<scene->entity_group_num;i++){
        group = ((GESceneEntityGroup*)scene->entity_groups.ptr)+i;
        ge_arena_free(&group->model_mat);
        ge_arena_free(&group->normal_mat);
        ge_arena_free(&group->entities);
    }
    ge_arena_free(&scene->entity_groups);
}

