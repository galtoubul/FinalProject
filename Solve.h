#include "Game.h"

/**
 * Solve Summary:
 *
 * Contains the functions for solving a board or getting information related to the solution
 * Main Functions:
 *
 * numSolutions  -  returns the number of solution to a given game board
 * generateILP   -  Generates a puzzle by randomly filling X empty cells with legal values,
 *                  running ILP to solve the board and then clearing all but Y random cells
 * isSolvable    -  Returns 1 if the board is solvable. Otherwise, returns 0.
 * guessLP       -  Guesses a solution to the current board using LP
 * guessHintLP   -  giving a hint to the user by showing the solution of a single cell
 * autofill      -  fills "obvious" values - cells which contain a single value
 */


/**
 * Calculates and returns the number of solutions to the given game board.
 * In doing so, uses an exhaustive backtracking algorithm using a stack instead of recursion.
 *
 * @param game - contains the board which we are calculating its solutions' number
 * @return number of possible solutions to the given game
 */
int numSolutions(Game* game);

/**
 * Generates a puzzle by randomly filling X empty cells with legal values,
 * running ILP to solve the board and then clearing all but Y random cells
 *
 * @param game  -  contains the board which we are trying to generate
 * @param X     -  number of cells which will be randomly filled with legal values
 * @param Y     -  number of non empty cells at the end of the process
 * @return  1 if succeeded to generate the board. Otherwise, returns 0.
 */
int generateILP(Game* game, int X, int Y);

/**
 * Validates the current board using ILP, ensuring it is solvable.
 *
 * @param game  -  contains the board which we validates
 * @return 1 if the board is solvable. Otherwise, returns 0.
 */
int isSolvable(Game* game);

/**
 * Guesses a solution to the current board using LP
 *
 * @param game       -  contains the board which we are trying to "guess"
 * @param threshold  -  a float represents the threshold we use for the LP,
 *                      i.e, fills a cell with a score of X or greater
 */
void guessLP (Game* game, double threshold);

/**
 * Giving a hint to the user by showing the solution of a single cell
 *
 * @param game  -  contains the board which we are trying to "guess" its cell values
 * @param x     -  the row index of the cell
 * @param y     -  the column index of the cell
 * @return 1 if the board is solvable -> the cell has at least one legal value. Otherwise, returns 0.
 */
int guessHintLP (Game* game, int x, int y);

/**
 * fills "obvious" values - cells which contain a single value
 *
 * @param game - contains the board which we are trying to autofill
 */
void autofill (Game* game);


/* --------------------------Assistance functions for numSolutions ---------------------- */

/**
 * Calculates the next cell at the board
 *
 * @param game  -  contains the board which we stared with and calculating its solutions' number
 * @param row   -  the current row. It will contain the row index of the next cell.
 * @param col   -  the current col. It will contain the column index of the next cell.
 */
void calcNextRowAndCol(Game* game, int* row, int* col);

/**
 * Calculates the first empty cell at the board
 *
 * @param game  -   contains the board which we stared with and calculating its solutions' number
 * @param row   -   first row.  It will contain the row index of the first empty cell.
 * @param col   -   first column.  It will contain the column index of the first empty cell.
 */
void findFirstEmptyCell (Game* game, int* row, int* col);

/**
 * Calculates the previous cell at the board
 *
 * @param game  -  contains the board which we stared with and calculating its solutions' number
 * @param row   -  the current row. It will contain the row index of the previous cell.
 * @param col   -  the current col. It will contain the column index of the previous cell.
 */
void calcPrevRowAndCol(Game* game, int* row, int* col);

/**
 * Calculates the last edited cell at the board
 *
 * @param curBoard  -  the board which we are working on
 * @param game      -  contains the board which we stared with and calculating its solutions' number
 * @param row       -  last row.  It will contain the row index of the last edited cell at curBoard.
 * @param col       -  last column.  It will contain the column index of the last edited cell at curBoard.
 */
void findLastEditedCell (int** curBoard, Game* game, int* row, int* col);

/**
 * Freeing curBoard
 *
 * @param curBoard  -  the board which we are working on
 * @param game      -  contains the board which we stared with and calculating its solutions' number
 */
void destroyCurBoard(int** curBoard, Game* game);

/* --------------------------Assistance functions for generateILP ---------------------- */

/**
 * Randomly filling X empty cells at the given board with legal values
 *
 * @param board  -  the board which we are working on
 * @param game   -  contains the board which we stared with and trying to chooseAndFillX
 * @param X      -  the number of cells we have to fill
 * @return 1 if succeeded to fill to fill X cells. Otherwise, returns 0.
 */
int chooseAndFillX (int** board, Game* game, int X);

 /**
  * Randomly choosing Y cells and clearing all other cells
  *
  * @param board  -  the board which we are working on
  * @param game   -  contains the board which we stared with and trying to chooseYCellsAndClearTheRest
  * @param Y      -  the number of cells which will be non empty at finish
  */
void chooseYCellsAndClearTheRest(int** board, Game* game, int Y);

/**
 * Calculates the array which contains the legal values for cell (x,y)
 *
 * @param board      -  the board which the function is trying to solve
 * @param game       -  contains the initial board which we stared with and trying to solve
 * @param x          -  the row's index of the checked cell
 * @param y          -  the column's index of the checked cell
 * @param legalArray -  an empty array which will contain the the legal values for cell (x,y) after function's finish
 * @return the actual size of the legal array after function's finish
 */
int getLegalArray (int **board, Game* game, int x, int y, int *legalArray);

/**
 * Deletes the given index from the given array
 *
 * @param array   -  the array which we are asking to remove an index from
 * @param size    -  the array's size
 * @param index   -  the removed index
 */
void deleteInd(int** array, int size, int index);

/**
 * @param legalArraySize - the size of legalArray
 * @return a random number between 0 and legalArraySize-1
 */
int getRand(int legalArraySize);

