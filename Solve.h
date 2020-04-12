#include "Game.h"

/**
 * Solve Summary:
 *
 * Contains the functions for solving a board or getting information related to the solution
 *
 * numSolutions  - return the number of solution to a given game board
 * solveBoard      - tries to solve a board from a given state
 * getLegalArray   - updates the legal values for a cell in its legalArray
 * deleteIndex     - deletes an index from legalArray
 * getRand         - returns a random number in a given range
 * calcNextI       - returns the row index of the next cell
 * calcNextJ       - returns the column index of the next cell
 *
 */

/**
 * Calculates and returns the number of solutions to the given game board.
 *
 * @param game - the target game
 * @return number of possible solutions to the given game
 */
int numSolutions(Game* game);

void calcNextRowAndCol(Game* game, int* row, int* col);

void findFirstEmptyCell (Game* game, int* row, int* col);

void calcPrevRowAndCol(Game* game, int* row, int* col);

void findLastEditedCell (int** curBoard, Game* game, int* row, int* col);



void destroyCurBoard(int** curBoard, Game* game);



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
 * @param legalArraySize - the size of legalArray
 * @return a random number between 0 and legalArraySize-1
 */
int getRand(int legalArraySize);



int generateILP(Game* game, int X, int Y);

int isSolvable(Game* game);

void autofill (Game* game);

void guessLP (Game* game, double threshold);

int guessHintLP (Game* game, int x, int y);
