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

#include <mibiengine2/base/modelarray.h>

#include <backendlist.h>

#include <mibiengine2/errors.h>

int ge_modelarray_init(GEModelArray *array, void *data, GEType type,
                       size_t size, size_t item_size, int updatable) {
    return GE_BACKENDLIST_GET(modelarray_init)(array, data, type, size,
                                               item_size, updatable);
}

int ge_modelarray_update(GEModelArray *array, void *data, size_t size) {
    return GE_BACKENDLIST_GET(modelarray_update)(array, data, size);
}

int ge_modelarray_enable(GEModelArray *array, GEModelArrayAttr *attr) {
    return GE_BACKENDLIST_GET(modelarray_enable)(array, attr);
}

int ge_modelarray_disable(GEModelArray *array) {
    return GE_BACKENDLIST_GET(modelarray_disable)(array);
}

void ge_modelarray_free(GEModelArray *array) {
    GE_BACKENDLIST_GET(modelarray_free)(array);
}

