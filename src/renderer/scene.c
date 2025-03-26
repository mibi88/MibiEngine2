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

#include <stdlib.h>

int ge_scene_init(GEScene *scene, GEEntity *entities, size_t entity_num,
                  size_t light_max) {
    size_t i, n;
    int found;
    void *new;
    scene->entityarray = malloc(GE_SCENE_ALLOC_STEP*
                                sizeof(GESceneEntityArray*));
    scene->renderable_num = 0;
    scene->renderable_max = GE_SCENE_ALLOC_STEP;
    for(i=0;i<entity_num;i++){
        found = 0;
        for(n=0;n<scene->renderable_num;n++){
            if(scene->entityarray[i].renderable == entities[i].data){
                /* Add an entity */
                /* TODO */
            }
        }
        if(!found){
            if(scene->renderable_num >= GE_SCENE_ALLOC_STEP){
                new = realloc(scene->entityarray, (scene->renderable_max+
                              GE_SCENE_ALLOC_STEP)*
                              sizeof(GESceneEntityArray*));
                if(new == NULL){
                    ge_scene_free(scene);
                    return 1;
                }
                scene->entityarray = new;
            }
            scene->entityarray[scene->renderable_num].entities =
                                                    malloc(sizeof(GEEntity));
            if(scene->entityarray[scene->renderable_num].entities == NULL){
                ge_scene_free(scene);
                return 2;
            }
            *scene->entityarray[scene->renderable_num].entities = entities[i];
            scene->entityarray[scene->renderable_num].renderable =
                                                            entities[i].data;
            scene->renderable_num++;
        }
    }
    scene->light_max = light_max;
    return 0;
}

void ge_scene_free(GEScene *scene) {
    (void)scene;
    /* TODO: Free everything */
}

