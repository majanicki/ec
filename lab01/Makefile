ifeq ($(OS),Windows_NT)
    RAYLIB = lib_win/libraylib.a -lm -lgdi32 -lwinmm
else
    RAYLIB = ./lib_linux/libraylib.a -lm
endif

all:
	g++ main.cpp -o main -Wall -Wextra -O3 -Iinclude ${RAYLIB}
