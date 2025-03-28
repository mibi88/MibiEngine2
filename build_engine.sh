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

NAME=MibiEngine2
VERSION="v.0.1"
SRCFILES=(src/backends/*.c src/backends/gles/*.c
          src/base/*.c src/renderer/*.c)
CC=cc
AR=ar
DEST=build
CFLAGS=(-ansi -Wall -Wextra -Wpedantic
        -Isrc/backends -Iinclude -g)

rebuild=false
clean=false

usage() {
    echo "Usage: $0 -rch"
    exit 0
}

while getopts "rch" flag; do
    case "${flag}" in
        r) rebuild=true ;;
        c) clean=true ;;
        h) usage ;;
        *) usage ;;
    esac
done

# TODO: Do not require to fully rebuild the project
if ! $rebuild; then
    echo "-- Warning: Rebuilding on changes isn't properly supported yet!"
    rebuild=true
fi

if $clean; then
    echo "-- Removing old build files"
    rm -f $DEST/*.o
    rm -f $DEST/*.a
    exit 0
fi

echo "-- Creating build folder..."

mkdir -p $DEST

objfiles=()

echo "-- Building $NAME..."
for file in ${SRCFILES[@]}; do
    base="${file##*/}"
    obj="$DEST/${base%%.*}.o"
    if [ $file -nt $obj ] || $rebuild; then
        echo "-- Compiling $file..."
        $CC -c -o $obj $file ${CFLAGS[@]}
        rc=$?
        if [ $rc -ne 0 ]; then
            echo "-- Build failed with return code $rc!"
            exit 1
        fi
    else
        echo "-- Skipping $file: file already compiled..."
    fi
    objfiles+=($obj)
done

echo "-- Archive $NAME.a..."
$AR rsv $DEST/$NAME.a ${objfiles[@]}

