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

#ifndef GE_BASE_H
#define GE_BASE_H

/* base.h
 *
 * base.h contains macros to help the creation of structs that can be extended
 * similarly to the GEModel struct (see model.h).
 *
 * Such types should be defined as following:
 * GE_<type>_INHERIT_MAX should be defined to the max. number of times that
 * this struct can be extended.
 * GE_<type>_INHERIT_LEVEL should be defined to -1, because the struct just got
 * defined here so it needs no extra data.
 * It should also contain callbacks to make it possible to extend (see
 * BASE_DATA).
 *
 * The function that initializes it should take an extra argument to set the
 * extra value of the struct stored in the extra[0] field.
 *
 * When extending this struct, a new GE_<new type>_INHERIT_LEVEL should be
 * defined for the extended version. Its initialization function should also
 * take an extra argument in it's initialization function to allow it to be
 * extended further. It should have its own struct, which is allocated in its
 * intialization function and put into extra.
 * it can then get its data in extra[GE_<new type>_INHERIT_LEVEL].
 * At the en of the initialization function the extra data should be set as
 * following:
 *
 * #if GE_<new type>_INHERIT_LEVEL+1 >= GE_<type>_INHERIT_MAX
 *     Stop compiling right now!
 * #endif
 *     model->extra[GE_<new type>_INHERIT_LEVEL+1] = extra;
 *
 * if the inherit level of the extended type is too high the compiler will
 * crash because of the plain text inside of the code, which will avoid buffer
 * overflows if a type got extended too often.
 */

/* BASE_DATA
 *
 * Define the callbacks in an extendable struct (see base.h).
 *
 * type:      The type name of the struct (in all caps, the same as in
 *            GE_<type>_INHERIT_MAX and GE_<type>_INHERIT_LEVEL).
 * callbacks: All the callbacks between two braces.
 */
#define BASE_DATA(type, callbacks) \
    struct callbacks calls[GE_##type##_INHERIT_MAX]; \
    size_t call_ptr; \
    void *extra[GE_##type##_INHERIT_MAX]

#endif

