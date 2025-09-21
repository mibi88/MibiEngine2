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

#include <mibiengine2/base/utils.h>

#include <mibiengine2/renderer/entity.h>

#include <mibiengine2/errors.h>

#include <stdlib.h>
#include <string.h>

int ge_scene_init(GEScene *scene, GEEntity *entities, size_t entity_num,
                  GEStdShader **shaders, size_t shader_num, size_t light_max) {
    scene->entity_group_num = 0;
    /* Initialize the entity group arena */
    if(ge_array_init(&scene->entity_groups, 0, sizeof(GESceneEntityGroup),
                     NULL)){
        ge_scene_free(scene);
        return GE_E_ARENA_INIT;
    }

    ge_scene_add_entities(scene, entities, entity_num);

    scene->shaders = shaders;
    scene->shader_num = shader_num;
    scene->light_max = light_max;
    return GE_E_NONE;
}

int _ge_scene_sort_groups(const void *_group1, const void *_group2) {
    const GESceneEntityGroup *group1 = _group1;
    const GESceneEntityGroup *group2 = _group2;
    return group2->renderable->priority-group1->renderable->priority;
}

int ge_scene_add_entities(GEScene *scene, GEEntity *entities,
                          size_t entity_num) {
    size_t i, n;
    int found;
    int group_added = 0;
    GESceneEntityGroup *group;
    /* TODO: Optimize this */
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
            GESceneEntityGroup _group;
            if(ge_array_add(&scene->entity_groups, &_group, 1)){
                ge_scene_free(scene);
                return GE_E_OUT_OF_MEM;
            }
            group = (GESceneEntityGroup*)scene->entity_groups.ptr+
                    scene->entity_groups.count-1;

            /* Add an entity group */
            group->entity_num = 0;
            group->renderable = entities[i].data;
            if(ge_array_init(&group->model_mat, 0, sizeof(GEMat4), NULL)){
                ge_scene_free(scene);
                return GE_E_ARENA_INIT;
            }
            if(ge_array_init(&group->normal_mat, 0, sizeof(GEMat3), NULL)){
                ge_array_free(&group->model_mat);
                ge_scene_free(scene);
                return GE_E_ARENA_INIT;
            }
            if(ge_array_init(&group->entities, 0, sizeof(GEEntity), NULL)){
                ge_array_free(&group->model_mat);
                ge_array_free(&group->normal_mat);
                ge_scene_free(scene);
                return GE_E_ARENA_INIT;
            }
            scene->entity_group_num++;
            group_added = 1;
        }
        /* Add the entity to the entity group pointed to by group */
        if(ge_array_add(&group->model_mat, &entities[i].model_mat, 1)){
            ge_scene_free(scene);
            return GE_E_ARENA_ALLOC;
        }
        if(ge_array_add(&group->normal_mat, &entities[i].normal_mat, 1)){
            ge_scene_free(scene);
            return GE_E_ARENA_ALLOC;
        }
        if(ge_array_add(&group->entities, entities+i, 1)){
            ge_scene_free(scene);
            return GE_E_OUT_OF_MEM;
        }
        group->entity_num++;
    }
    if(group_added){
        /* Sort the entity groups by renderable priority */
        if(ge_utils_sort(scene->entity_groups.ptr, scene->entity_group_num,
                         sizeof(GESceneEntityGroup), _ge_scene_sort_groups)){
            return GE_E_SORT;
        }
    }
    return GE_E_NONE;
}

void ge_scene_set_shaders(GEScene *scene, GEStdShader **shaders,
                          size_t shader_num) {
    scene->shaders = shaders;
    scene->shader_num = shader_num;
}

void ge_scene_set_camera(GEScene *scene, GECamera *camera) {
    scene->camera = camera;
}

GEEntity *ge_scene_get_same_entity(GEScene *scene, GEEntity *entity) {
    size_t i, n;
    GESceneEntityGroup *group;
    for(i=0;i<scene->entity_group_num;i++){
        group = ((GESceneEntityGroup*)scene->entity_groups.ptr)+i;
        for(n=0;n<group->entity_num;n++){
            if(memcmp(entity, (GEEntity*)group->entities.ptr+n,
                      sizeof(GEEntity))){
                return (GEEntity*)group->entities.ptr+n;
            }
        }
    }
    return NULL;
}

void ge_scene_for_entity(GEScene *scene,
                         void on_entity(GEEntity *entity, void *data),
                         void *data) {
    size_t i, n;
    GESceneEntityGroup *group;
    for(i=0;i<scene->entity_group_num;i++){
        group = ((GESceneEntityGroup*)scene->entity_groups.ptr)+i;
        for(n=0;n<group->entity_num;n++){
            on_entity((GEEntity*)group->entities.ptr+n, data);
        }
    }
}

void ge_scene_for_entity_with_renderable(GEScene *scene,
                                         GERenderable *renderable,
                                         void on_entity(GEEntity *entity,
                                                        void *data),
                                         void *data) {
    size_t i, n;
    GESceneEntityGroup *group;
    for(i=0;i<scene->entity_group_num;i++){
        group = ((GESceneEntityGroup*)scene->entity_groups.ptr)+i;
        if(group->renderable == renderable){
            for(n=0;n<group->entity_num;n++){
                on_entity((GEEntity*)group->entities.ptr+n, data);
            }
            break;
        }
    }
}

void ge_scene_update(GEScene *scene) {
    size_t i, n;
    GESceneEntityGroup *group;
    GEEntity *entity;
    for(i=0;i<scene->entity_group_num;i++){
        group = ((GESceneEntityGroup*)scene->entity_groups.ptr)+i;
        for(n=0;n<group->entity_num;n++){
            entity = (GEEntity*)group->entities.ptr+n;
            ((GEMat4*)group->model_mat.ptr)[n] = entity->model_mat;
            ((GEMat3*)group->normal_mat.ptr)[n] = entity->normal_mat;
        }
    }
}

void ge_scene_render(GEScene *scene) {
    size_t i;
    GESceneEntityGroup *group;
    if(scene->camera){
        for(i=0;i<scene->shader_num;i++){
            ge_camera_use(scene->camera, scene->shaders[i]);
        }
    }
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
        ge_array_free(&group->model_mat);
        ge_array_free(&group->normal_mat);
        ge_array_free(&group->entities);
    }
    ge_array_free(&scene->entity_groups);
}

