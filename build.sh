#!/usr/bin/env bash

set -e

RAYLIB="-I./raylib/include/ ./raylib/lib_linux/libraylib.a -lm"
SOLVERLIB="-I./lib/"
CXXFLAGS="-g3 -ggdb -Wall -Wextra"


SRC="./$1/main.cpp"
OUT="./$1/main"

clang++ -o $OUT $SRC ${SOLVERLIB} ${RAYLIB} ${CXXFLAGS}
