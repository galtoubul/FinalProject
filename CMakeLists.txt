cmake_minimum_required(VERSION 3.15)
project(Sudoku)

set(CMAKE_C_STANDARD 99)
set(GCC_COVERAGE_COMPILE_FLAGS "-ansi -Wall -Wextra -Werror -pedantic-errors")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
add_executable(Sudoku main.c Game.c Game.h Parser.c Parser.h MainAux.c MainAux.h Solve.c Solve.h)