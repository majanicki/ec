ifeq ($(OS),Windows_NT)
    RAYLIB= -I./raylib/include/ raylib/lib_win/libraylib.a  -lm -lgdi32 -lwinmm
else
    RAYLIB= -I./raylib/include/ ./raylib/lib_linux/libraylib.a -lm
endif

SOLVERLIB= -I./lib/
CXXFLAGS=-O3 -Wall -Wextra

lab01:
	g++ -o lab01 ./lab01/main.cpp ${SOLVERLIB} ${RAYLIB} ${CXXFLAGS}

.PHONY: lab01
