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

int ge_scene_init(GEScene *scene, GEEntity *entities, size_t entity_num,
                  size_t light_max) {
    size_t i, n;
    int found;
    void *new;
    scene->entity_array = malloc(GE_SCENE_ALLOC_STEP*
                                sizeof(GESceneEntityArray*));
    scene->renderable_num = 0;
    scene->renderable_max = GE_SCENE_ALLOC_STEP;
    for(i=0;i<entity_num;i++){
        found = 0;
        for(n=0;n<scene->renderable_num;n++){
            if(scene->entity_array[i].renderable == entities[i].data){
                /* Add an entity */
                if(scene->entity_array[i].entity_num >=
                   scene->entity_array[i].entity_max){
                    scene->entity_array[i].entity_max += GE_SCENE_ALLOC_STEP;
                    new = realloc(scene->entity_array[i].entities,
                                  scene->entity_array[i].entity_max*
                                  sizeof(GEEntity*));
                    if(new == NULL){
                        ge_scene_free(scene);
                        return GE_E_OUT_OF_MEM;
                    }
                    scene->entity_array[i].entities = new;
                }
                scene->entity_array[i]
                    .entities[scene->entity_array[i].entity_num] = entities[i];
                scene->entity_array[i].entity_num++;
            }
        }
        if(!found){
            if(scene->renderable_num >= scene->renderable_max){
                scene->renderable_max += GE_SCENE_ALLOC_STEP;
                new = realloc(scene->entity_array, scene->renderable_max*
                              sizeof(GESceneEntityArray*));
                if(new == NULL){
                    ge_scene_free(scene);
                    return GE_E_OUT_OF_MEM;
                }
                scene->entity_array = new;
            }
            scene->entity_array[scene->renderable_num].entities =
                                                    malloc(sizeof(GEEntity));
            if(scene->entity_array[scene->renderable_num].entities == NULL){
                ge_scene_free(scene);
                return GE_E_OUT_OF_MEM;
            }
            *scene->entity_array[scene->renderable_num].entities = entities[i];
            scene->entity_array[scene->renderable_num].renderable =
                                                            entities[i].data;
            scene->renderable_num++;
        }
    }
    scene->light_max = light_max;
    return GE_E_SUCCESS;
}

void ge_scene_render(GEScene *scene) {
    size_t i, n, x;
    size_t count;
    GEMat4 *model_mats[GE_SCENE_INSTANCING_MAX];
    GEMat3 *normal_mats[GE_SCENE_INSTANCING_MAX];
    for(i=0;i<scene->renderable_num;i++){
        for(n=0;n<scene->entity_array[i].entity_num;
            n+=GE_SCENE_INSTANCING_MAX){
            count = scene->entity_array[i]
                        .entity_num-n > GE_SCENE_INSTANCING_MAX ?
                    GE_SCENE_INSTANCING_MAX : scene->entity_array[i]
                        .entity_num-n;
            for(x=0;x<count;x++){
                model_mats[x] = &scene->entity_array[i].entities[x].mat;
            }
            for(x=0;x<count;x++){
                normal_mats[x] = &scene->entity_array[i].entities[x]
                                     .normal_mat;
            }
            ge_renderable_render_multiple(scene->entity_array[i].renderable,
                                          model_mats, normal_mats, count);
        }
    }
}

void ge_scene_free(GEScene *scene) {
    size_t i;
    for(i=0;i<scene->renderable_max;i++){
        free(scene->entity_array[i].entities);
        scene->entity_array[i].entities = NULL;
    }
    free(scene->entity_array);
    scene->entity_array = NULL;
}

