@echo off
setlocal

set RAYLIB=-I.\raylib\include\ raylib\lib_win\libraylib.a -lm -lgdi32 -lwinmm
set SOLVERLIB=-I.\lib\
set CXXFLAGS=-O3 -Wall -Wextra

g++ -o %1 .\%1\main.cpp %SOLVERLIB% %RAYLIB% %CXXFLAGS%

endlocal

