cmake_minimum_required(VERSION 3.15)
project(Sudoku)

set(CMAKE_C_STANDARD 90)
set(GCC_COVERAGE_COMPILE_FLAGS "-ansi -Wall -Wextra -Werror -pedantic-errors")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
add_executable(Sudoku main.c Game.c Game.h Parser.c Parser.h Solve.c Solve.h Stack.c Stack.h IO.c IO.h
        CmdHandler.c CmdHandler.h GameFlow.c GameFlow.h ErrorsInterface.h MODE.h
         EntryTable.c EntryTable.h LPSolver.h LPSolver.c)

include_directories( C:/gurobi801/win64/include )
find_package (C:/gurobi801/win64/lib/gurobi80.lib)

    include(CMakePackageConfigHelpers)
    include(CTest)
    include(GNUInstallDirs)

target_link_libraries (Sudoku C:/gurobi801/win64/lib/gurobi80.lib)