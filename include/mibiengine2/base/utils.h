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

#ifndef GE_UTILS_H
#define GE_UTILS_H

#include <stddef.h>

/* ge_utils_power_of_two
 *
 * Get the closest power of two to num.
 *
 * num: The number to get the closest power of two to.
 * Returns the closest power of two to num.
 */
int ge_utils_power_of_two(int num);

/* ge_utils_adler32
 *
 * Get the Adler-32 checksum of data.
 *
 * data: The data to get the Adler-32 checksum of.
 * size: The size of the data.
 * Returns the Adler-32 checksum.
 */
unsigned long int ge_utils_adler32(unsigned char *data, size_t size);

/* ge_utils_sort
 *
 * Sort the data in data in the ascending order.
 *
 * ge_utils_sort uses a stable sorting algorithm.
 *
 * data:      The data to sort.
 * size:      The number of elements in the data to sort.
 * item_size: The size of an item in the data.
 * cmp:       A function that compares two items. It should return a positive
 *            number if item1 > item2, 0 if the two items are equal or a
 *            negative number if item1 < item2.
 *
 * Returns GE_E_NONE (0) on success or an error code on failure.
 */
int ge_utils_sort(void *data, size_t size, size_t item_size,
                  int cmp(const void *item1, const void *item2));

#endif

