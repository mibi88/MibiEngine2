#!/bin/bash

# A small OpenGL ES engine.
# by Mibi88
#
# This software is licensed under the BSD-3-Clause license:
#
# Copyright 2025 Mibi88
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
# contributors may be used to endorse or promote products derived from this
# software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

echo "-- Building the engine..."

./build_engine.sh $1 $2 $3

rc=$?

if [ $rc -ne 0 ]; then
    exit $rc
fi
clean=false
emscripten=false

while getopts "rche" flag; do
    case "${flag}" in
        c) clean=true ;;
        e) emscripten=true ;;
    esac
done

if $clean; then
    rm build/*.html
    rm build/*.js
    rm build/*.wasm
    rm -rf build/assets
    exit $rc
fi

echo "-- Building the example..."

CC=gcc
BIN=main

CFLAGS=(-ansi -Iinclude -lEGL -lm -lX11 -lGL -lpng build/MibiEngine2.a -Wall \
        -Wextra -Wpedantic -g)

if $emscripten; then
    echo "-- Using emscripten!"
    CC=emcc
    CFLAGS=$CFLAGS" -sUSE_ZLIB=1 --embed-file build/assets@/"
    BIN=build/index.js
    mkdir -p build/assets
    cp -r shaders build/assets/shaders
    cp *.png build/assets
    cp *.obj build/assets
    cp data/index.html build
fi

$CC example/*.c -o $BIN ${CFLAGS[@]}

