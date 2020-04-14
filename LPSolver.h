#include "EntryTable.h"
#include "gurobi_c.h"

/**
 * LPSolver Summary:
 *
 * Contains the functions for solving a board using ILP or LP using Gurobi library
 *
 * LPSolver                  -  solves a board using LP or ILP
 * addConstraints            -  creating constraints for each cell, row, column and block using the following functions:
 * addConstraintsPerCell     -  creating constraints for each cell
 * addConstraintsPerRow      -  creating constraints for each  row
 * addConstraintsPerColumn   -  creating constraints for each column
 * addConstraintsPerBlock    -  creating constraints for each block
 * addConstraintsPerVariable -  creating constraints for each variable (relevant only at LP)
 */


/**
 * Solves a board using LP or ILP
 * Returns 1 if succeeded to solve the board. Otherwise, returns 0.
 *
 * game     -  contains the board which are trying to solve
 * et       -  contains an Entry Table for transforming between values at the board to Gurobi variables
 * sol      -  if the program manage to solve the board, then sol will contain its solution
 * varType  -  1(LP) for linear Programing, 0(ILP) for Integer Linear Programing
 */
int LPSolver(Game* game, EntryTable* et, double* sol, int varType);

/**
 * these parameters are identical to the following functions
 * (not all functions contain them all)
 *
 * @param ind    -   an array which contains the variables for constraints
 * @param val    -   an array which contains the coefficient for the variables at ind array
 * @param obj    -   an array which contains the coefficients for the variables at the objective function
 * @param vtype  -   the type of variables: binary/integer/float
 * @param model  -   the Gurobi model which are working on
 * @param env    -   the Gurobi environment which are working on
 * @param et     -   contains the information about transitions between Gurobi varialbles to values at the board game
 * @param game   -   contains the board which we are trying to solve
 * @param sol    -   will contain the values of the Gurobi variables after optimizing
 */

/**
 * Freeing: all allocated memory during LPSolver, model and environment
 */
void freeModel(int* ind, double* val, double* obj, char* vtype, GRBmodel* model, GRBenv* env, EntryTable* et, Game* game, double* sol);

/**
 * Relevant only to LP only
 * Creating constraints for each variable at the model s.t for each variable x: 0 <= x <= 1
 *
 * @param varType  -  1(LP) for linear Programing, 0(ILP) for Integer Linear Programing
 * @return 1 if succeeded to add constraints per variable. Otherwise, returns 0.
 */
int addConstraintsPerVariable(GRBmodel* model, int* ind, double* val, EntryTable* et, int varType);

/**
 * Creating constraints for each cell at the board s.t for each cell x: sum of values for x = 1
 * @return 1 if succeeded to add constraints per cell. Otherwise, returns 0.
 */
int addConstraintsPerCell(GRBmodel* model, int* ind, double* val, Game* game, EntryTable* et);

/**
 * Creating constraints for each row at the board s.t for each row i and for each value x: sum of x at row i = 1
 * @return 1 if succeeded to add constraints per row. Otherwise, returns 0.
 */
int addConstraintsPerRow(GRBmodel* model, int* ind, double* val, Game* game, EntryTable* et);

/**
 * Creating constraints for each column at the board s.t for each column j and for each value x: sum of x at row j = 1
 * @return 1 if succeeded to add constraints per column. Otherwise, returns 0.
 */
int addConstraintsPerColumn(GRBmodel* model, int* ind, double* val, Game* game, EntryTable* et);

/**
 * Creating constraints for each block at the board s.t for each block k and for each value x: sum of x at block k = 1
 * @return 1 if succeeded to add constraints per block. Otherwise, returns 0.
 */
int addConstraintsPerBlock(GRBmodel* model, int* ind, double* val, Game* game, EntryTable* et);

/**
 * Creating constraints for each cell, row, column and block. In LP adding also constraints per variable.
 * @return 1 if succeeded to add all constraints. Otherwise, returns 0.
 */
void addConstraints(GRBmodel* model, int* ind, double* val, Game* game, EntryTable* et, int varType,
                    double* obj, char* vtype, double* sol, GRBenv* env);
