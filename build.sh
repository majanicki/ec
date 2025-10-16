#!/usr/bin/env bash

set -e

RAYLIB="-I./raylib/include/ ./raylib/lib_linux/libraylib.a -lm"
SOLVERLIB="-I./lib/"
CXXFLAGS="-g -Wall -Wextra"

g++ -o $0 ./$0/main.cpp ${SOLVERLIB} ${RAYLIB} ${CXXFLAGS}
