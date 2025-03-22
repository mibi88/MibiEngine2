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

/* TODO: This blur is quite messy, I should render it in two passes */

/* Got it from python:
 * import math as m; print(m.pi)
 */
#define PI 3.141592653589793

/* Kernel generated with the following python code:
 * import math as m; s=3; r=16;
 * print([1/m.sqrt(2*m.pi*s*s)*m.exp(-0.5*(x/2)*(x/2)/(2*s*s))
 *       for x in range(-r, r+1)])
 */

#define STEP (1.0/2000.0)
#define KERNEL_RADIUS 0
varying vec2 frag_pos;
varying vec2 frag_uv;

uniform sampler2D color;
uniform sampler2D depth;

float gauss(float x, float s) {
    return 1.0/sqrt(2.0*PI*s*s)*exp(-x*x/(2.0*s*s));
}

void main() {
#if KERNEL_RADIUS
    float kernel[2*KERNEL_RADIUS*2*KERNEL_RADIUS];
    float m = 0.0;
    float v;
    vec4 c;
    vec2 pos;
    gl_FragColor = vec4(0.0);
    for(int y=-KERNEL_RADIUS;y<KERNEL_RADIUS;y++){
        for(int x=-KERNEL_RADIUS;x<KERNEL_RADIUS;x++){
            kernel[(y+KERNEL_RADIUS)*2*KERNEL_RADIUS+(x+KERNEL_RADIUS)] =
                gauss(float(x)/(3.0*float(KERNEL_RADIUS)),
                      float(KERNEL_RADIUS))*
                gauss(float(y)/(3.0*float(KERNEL_RADIUS)),
                      float(KERNEL_RADIUS));
        }
    }
    for(int i=0;i<2*KERNEL_RADIUS*2*KERNEL_RADIUS;i++){
        m += kernel[i];
    }
    for(int i=0;i<2*KERNEL_RADIUS*2*KERNEL_RADIUS;i++){
        kernel[i] /= m;
    }
    for(int y=-KERNEL_RADIUS;y<KERNEL_RADIUS;y++){
        for(int x=-KERNEL_RADIUS;x<KERNEL_RADIUS;x++){
            v = kernel[(y+KERNEL_RADIUS)*2*KERNEL_RADIUS+(x+KERNEL_RADIUS)];
            pos = vec2(frag_uv.x+float(x)*STEP, frag_uv.y+float(y)*STEP);
            c = texture2D(color, pos);
            gl_FragColor += c*v;
        }
    }
#else
    gl_FragColor = texture2D(color, frag_uv);
#endif
}

