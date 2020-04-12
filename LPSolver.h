#include "EntryTable.h"

/**
 * ILP Summary:
 *
 * Contains the functions for solving a board using ILP or LP using Gurobi library
 *
 * LPSolver  - solves a board using LP or ILP
 * addConstraints            -  creating constraints for each cell, row, column and block using the following functions:
 * addConstraintsPerCell     -  creating constraints for each cell
 * addConstraintsPerRow      -  creating constraints for each  row
 * addConstraintsPerColumn   -  creating constraints for each column
 * addConstraintsPerBlock    -  creating constraints for each block
 * addConstraintsPerVariable -  creating constraints for each variable (relevant only at LP)
 */

/**
 * Solves a board using LP or ILP
 * Returns 1 if succeeded to solve the board. Otherwise, returns 0
 *
 * game     -  contains the board which are trying to solve
 * et       -  contains an Entry Table for trasforming between values at the board to Gurobi variables
 * sol      -  if the program manage to solve the board, then sol will contain its solution
 * varType  -  1(LP) for linear Programing, 0(ILP) for Integer Linear Programing
 */
int LPSolver(Game* game, EntryTable* et, double* sol, int varType);
