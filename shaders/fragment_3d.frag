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

#version 100
precision lowp float;

varying vec4 frag_pos;
varying vec4 frag_color;
varying vec3 frag_uv;
varying vec3 frag_normal;

uniform sampler2D tex;
uniform vec2 uv_max;

vec3 light = vec3(0.0);
vec3 camera_pos = vec3(0.0);
float ambient_lighting = 0.3;
float specular_strength = 1.0;
float shininess = 32.0;

float diffuse_lighting(vec3 light) {
    return max(dot(frag_normal, normalize(light-frag_pos.xyz)), 0.0);
}

float specular_lighting(vec3 light) {
    return pow(max(dot(normalize(camera_pos-frag_pos.xyz),
                       reflect(-normalize(light-frag_pos.xyz), frag_normal)),
                       0.0), shininess)*specular_strength;
}

float lighting(vec3 light) {
    return diffuse_lighting(light)+specular_lighting(light)+ambient_lighting;
}

void main() {
    vec2 pos = mod((frag_uv.xy*uv_max), uv_max);
    pos.y = uv_max.y-pos.y;
    gl_FragColor = texture2D(tex, pos)*
                   lighting(light);
}

