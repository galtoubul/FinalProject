#include "Game.h"

/**
 * Stack Summary:
 *
 * Contains the struct of Entry Table and its related functions:
 *
 * createEntryTable    -  creates a new Entry Table and initializes its fields
 * calcVariables       -  an assistance function for createEntryTable.
 * destroyEntryTable   -  Frees all allocated memory related to the stack.
 * parseSol            -  inserts a Game into the top of the stack.
 * parseLPSol          -  returns the Game from the top of the stack and removes it from the Entry Table.
 * printEntryTable     -  for debugging purposes. Prints all relevant information about the entry table.
 */


/**
 * struct of Entry Table:
 *
 * variablesMat - a 3D array which maps each legal value at the game board into a variable
 * for example:  for the board:         we will get the following variablesMat:
 *           -----------------          -------------------------------------------------
 *           |___ _3_|___ _1_|          |__  2 __  4|__ __ __ __| |__ 10 __ 12|__ __ __ __|
 *           |___ ___|___ ___|          |17 18 __ 20|21 22 __ 24| |__ 26 27 28|__ 30 31 32|
 *           -----------------          ------------------------------------------------
 *           |___ ___|___ ___|          |33 34 35 36|37 38 __ 40| |41 42 43 44|__ 46 47 48|
 *           |___ ___|___ ___|          |49 50 51 52|53 54 __ 56| |57 58 59 60|__ 62 63 64|
 *           -----------------          -------------------------------------------------
 *
 * varToInd - a 2D array s.t varToInd[i][0] = the board's row index of variable i + 1
 *                           varToInd[i][1] = the board's column index of variable i + 1
 *            for example: at the above example, for variable #10: varToInd[10-1][0] = 0, varToInd[10-1][1] = 2
 *
 * gurobiToVariablesMat - an array for transition between Gurobi's index to variableMat's index
 *                        s.t the index at the array is the Gurobi's index and its value is the variableMat's index
 * variablesMatToGurobi - the opposite way to gurobiToVariablesMat,
 *                        just that variableMat #x is in x-1 index at the array (because variableMat starts from 1)
 * variablesNum - number of variables (only variable for legal values are counted)
 * maxVariableNum - max number of variables (variablesNum == maxVariableNum iff the board is empty)
 * possibleValuesPerCell - numbers of possible values for a cell at the given board, without limitations
 */
typedef struct Entry_Table{
    int*** variablesMat;
    int** varToInd;
    int* gurobiToVariablesMat;
    int* variablesMatToGurobi;
    int variablesNum;
    int maxVariableNum;
    int possibleValuesPerCell;
}EntryTable;

/**
 * Allocates memory for variablesMat and varToInd
 * Initializes varToInd, maxVariableNum and possibleValuesPerCell
 * @param game - contains the board
 * @return pointer to the created Entry Table
 */
EntryTable* createEntryTable(int** board, Game* game);

/**
 * Calculate the variables of variablesMat for the given board, and according to calculations,
 * initializes variablesNum and allocates memory for gurobiToVariablesMat and variablesMatToGurobi and initialing them
 */
void calcVariables (int** board, Game* game, EntryTable* et);

/**
 * Frees all allocated memory related to the given stack
 */
void destroyEntryTable(EntryTable* et, Game* game);

/**
 * Used for parsing the solution of ILP
 * Given the solution (at sol), uses the entry table to decide which value to assign for each cell at board
 */
void parseSol (int** board, EntryTable* et, double* sol);

/**
 * Used for parsing the solution of LP
 * Given the solution (at sol), uses the entry table to decide which value to assign for each cell at board
 * Only values with result that equal or greater than X will be transitioned into board
 * If more than 1 value is appropriate, so the function will randomize one of them
 */
void parseLPSol (Game* game, int** board, EntryTable* et, double* sol, double threshold);

/**
 * For debugging purposes
 * Prints all relevant information about the entry table:
 * variablesNum, variablesMat, varToInd, gurobiToVariablesMat, variablesMatToGurobi,
 */
void printEntryTable (EntryTable* et, Game* game);
