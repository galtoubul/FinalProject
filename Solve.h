#include "Game.h"

void calcNextRowAndCol(Game* game, int* row, int* col);

void findFirstEmptyCell (Game* game, int* row, int* col);

void calcPrevRowAndCol(Game* game, int* row, int* col);

void findLastEditedCell (Game* game, int* row, int* col);
/*
Mat* copyMat (Mat* otherMat);
*/

/**
 * Calculates and returns the number of solutions to the given game.
 *
 * @param game - the target game
 * @return number of possible solutions to the given game
 */
int num_solutions(Game* game);

/* --------------------------------------------- EX3 --------------------------------------------------------- */

/**
 * Solve Summary:
 *
 * Contains the functions for solving a board
 *
 * generatePuzzle  - solves an empty board
 * solveBoard      - tries to solve a board from a given state
 * getLegalArray   - updates the legal values for a cell in its legalArray
 * deleteIndex     - deletes an index from legalArray
 * getRand         - returns a random number in a given range
 * calcNextI       - returns the row index of the next cell
 * calcNextJ       - returns the column index of the next cell
 *
 */

/**
 * Solves an empty board. When finished the solution will be available in game->solutionBoard.
 *
 * @param game - the target game
 * @param i - starting row index
 * @param j - starting column index
 * @return 1 - if it managed to solve the board. Otherwise - returns 0
 */
int generatePuzzle(Game *game, int i, int j);

/**
 * Tries to solve a board from a given state
 * If it managed to solve it, then the solution will be available in game->solutionBoard.
 *
 * @param game - the target game
 * @param mat - a copy of the original board which the function is trying to solve
 * @param i - starting row index
 * @param j - starting column index
 * @return 1 - if it managed to solve the board. Otherwise - returns 0
 */
int solveBoard(Game *game, int **mat, int i, int j);

/**
 *Calculates the array which contains the legal values for cell (x,y)
 *
 * @param mat - the board which the function is trying to solve
 * @param x - the row's index of the checked cell
 * @param y - the column's index of the checked cell
 * @param legalArray - an empty array which will contain the the legal values for cell (x,y) after function's finish
 * @return the actual size of the legal array after function's finish
 */
int getLegalArray (int **board, Game* game, int x, int y, int *legalArray);

/**
 * Deletes an index from legalArray
 *
 * @param legalArray - the array of legal values for the current cell
 * @param legalArraySize - the size of legalArray
 * @param index - the index of the cell which will be deleted from legalArray
 */
void deleteIndex(int *legalArray, int legalArraySize, int index);

/**
 * @param legalArraySize - the size of legalArray
 * @return a random number between 0 and legalArraySize-1
 */
int getRand(int legalArraySize);

/**
 * Calculates the row's index of the next cell
 *
 * @param game - the target game
 * @param i - the row's index of the current cell
 * @param j - the column's index of the current cell
 * @return the row's index of the next cell
 */
int calcNextI(Game *game, int i, int j);

/**
 * Calculates the column's index of the next cell
 *
 * @param game - the target game
 * @param i - the row's index of the current cell
 * @param j - the column's index of the current cell
 * @return the column's index of the next cell
 */
int calcNextJ(Game *game, int j);

int generateILP(Game* game, int X, int Y);

int isSolvable(Game* game);

void autofill (Game* game);

void guessLP (Game* game, double threshold);

int guessHintLP (Game* game, int x, int y);
